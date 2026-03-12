# QtPrism — 基于 C++/Qt/QML 的 Prism 框架移植方案

> 参考项目：[PrismLibrary/Prism](https://github.com/PrismLibrary/Prism)
> 目标平台：C++17 / Qt 6.x / QML
> 文档版本：v1.0

---

## 一、项目总览

### 1.1 目标

将 .NET Prism 框架的核心架构理念移植到 C++/Qt/QML 平台，构建一套完整的 **模块化应用框架**，使
Qt/QML 开发者能够以松耦合、可扩展的方式构建大型桌面/嵌入式应用。

### 1.2 范围界定

| 类别 | 内容 | 状态 |
|------|------|------|
| **核心移植** | IoC/DI 容器 | ✅ 实现 |
| **核心移植** | 模块/插件系统 (Modularity) | ✅ 实现 |
| **核心移植** | 界面容器系统 (Region) | ✅ 实现 |
| **核心移植** | 服务与服务接口设计 | ✅ 实现 |
| **核心移植** | 消息机制 (EventAggregator) | ✅ 实现 |
| **核心移植** | 导航系统 (Navigation) | ✅ 实现 |
| **调整移植** | 属性系统 → Qt Q_PROPERTY | ✅ 调整实现 |
| **排除** | Command (DelegateCommand/CompositeCommand) | ❌ 不实现 |
| **排除** | BindableBase | ❌ 不实现（Qt 原生替代） |
| **排除** | Dialogs (IDialogService) | ❌ 不实现 |

### 1.3 技术选型

| 技术 | 选择 | 说明 |
|------|------|------|
| 语言标准 | C++17 | 支持 `std::any`, `std::optional`, `if constexpr` 等特性 |
| Qt 版本 | Qt 6.5+ | 使用 CMake 构建，QML 类型注册使用声明式宏 |
| 构建系统 | CMake 3.21+ | Qt 6 推荐的构建系统 |
| 插件机制 | Qt Plugin System | `QPluginLoader` + 自定义 `IModule` 接口 |
| 反射替代 | Qt 元对象系统 | `QMetaObject`, `Q_PROPERTY`, `Q_INVOKABLE` |
| 智能指针 | `QSharedPointer` + `std::shared_ptr` | Qt 对象用前者，纯 C++ 对象用后者 |

---

## 二、架构总体设计

### 2.1 分层架构

```
┌─────────────────────────────────────────────────────────────┐
│                     Sample Application                       │
│                 (Shell + Feature Modules)                     │
├──────────┬──────────┬──────────┬──────────┬─────────────────┤
│ ModuleA  │ ModuleB  │ ModuleC  │  ...     │  (Qt Plugins)   │
├──────────┴──────────┴──────────┴──────────┴─────────────────┤
│                      QtPrism.Wpf (Qt实现层)                   │
│  RegionManager │ RegionAdapters │ Navigation │ ModuleManager  │
├─────────────────────────────────────────────────────────────┤
│                      QtPrism.Core (核心抽象层)                 │
│  IModule │ IRegion │ IRegionManager │ INavigation │ Services │
├─────────────────────────────────────────────────────────────┤
│                      QtPrism.Events (事件层)                   │
│              EventAggregator │ PubSubEvent<T>                │
├─────────────────────────────────────────────────────────────┤
│                  QtPrism.Container (IoC容器层)                 │
│         IContainerRegistry │ IContainerProvider               │
├─────────────────────────────────────────────────────────────┤
│                    Qt 6 / QML Runtime                         │
└─────────────────────────────────────────────────────────────┘
```

### 2.2 库/模块划分

| 库名 | 类型 | 职责 | 对应 Prism 程序集 |
|------|------|------|-------------------|
| `QtPrismContainer` | 静态库 | IoC/DI 容器抽象与实现 | `Prism.Container.Abstractions` |
| `QtPrismEvents` | 静态库 | EventAggregator 消息机制 | `Prism.Events` |
| `QtPrismCore` | 静态库 | 核心接口定义（模块、Region、导航、服务） | `Prism.Core` |
| `QtPrismFramework` | 动态库 | Qt/QML 平台实现（Region适配器、行为、模块管理器） | `Prism.Wpf` |
| `QtPrismApp` | 可执行 | 样例应用程序 Shell | 用户应用 |
| `ModuleXxx` | Qt 插件 | 各功能模块 | 用户模块 |

### 2.3 依赖关系图

```
QtPrismContainer ← QtPrismEvents ← QtPrismCore ← QtPrismFramework ← QtPrismApp
                                                                     ← ModuleA
                                                                     ← ModuleB
                                                                     ← ...
```

---

## 三、核心子系统详细设计

### 3.1 IoC/DI 容器 (`QtPrismContainer`)

#### 3.1.1 设计思路

由于 C++ 缺少 .NET 的运行时反射，容器设计采用：
- **类型擦除**：使用 `std::type_index` 作为类型键
- **工厂函数**：使用 `std::function<std::shared_ptr<void>()>` 存储创建逻辑
- **模板接口**：编译期类型安全的注册和解析 API

#### 3.1.2 核心接口

```cpp
// ==================== IContainerRegistry ====================
class IContainerRegistry
{
public:
    virtual ~IContainerRegistry() = default;

    // 注册瞬态类型（每次 resolve 创建新实例）
    template<typename TInterface, typename TImpl>
    void Register();

    // 注册单例类型
    template<typename TInterface, typename TImpl>
    void RegisterSingleton();

    // 注册已有实例
    template<typename TInterface>
    void RegisterInstance(std::shared_ptr<TInterface> instance);

    // 注册带名称的类型（用于导航视图解析）
    template<typename TInterface, typename TImpl>
    void Register(const QString& name);

    // 检查类型是否已注册
    template<typename T>
    bool IsRegistered() const;

    // 内部接口（非模板）
    virtual void RegisterFactory(std::type_index type,
                                 std::function<std::shared_ptr<void>()> factory,
                                 bool isSingleton = false) = 0;
    virtual void RegisterFactory(std::type_index type, const QString& name,
                                 std::function<std::shared_ptr<void>()> factory,
                                 bool isSingleton = false) = 0;
    virtual void RegisterInstance(std::type_index type,
                                  std::shared_ptr<void> instance) = 0;
    virtual bool IsRegistered(std::type_index type) const = 0;
};

// ==================== IContainerProvider ====================
class IContainerProvider
{
public:
    virtual ~IContainerProvider() = default;

    template<typename T>
    std::shared_ptr<T> Resolve();

    template<typename T>
    std::shared_ptr<T> Resolve(const QString& name);

    // 内部接口（非模板）
    virtual std::shared_ptr<void> Resolve(std::type_index type) = 0;
    virtual std::shared_ptr<void> Resolve(std::type_index type,
                                          const QString& name) = 0;
};

// ==================== IContainerExtension ====================
// 统一接口，同时继承注册和解析能力
class IContainerExtension : public IContainerRegistry, public IContainerProvider
{
public:
    virtual void FinalizeExtension() = 0;
};
```

#### 3.1.3 默认实现 `QtPrismContainer`

```cpp
class QtPrismContainerImpl : public IContainerExtension
{
private:
    struct Registration
    {
        std::function<std::shared_ptr<void>()> factory;
        bool isSingleton = false;
        std::shared_ptr<void> singletonInstance;
    };

    std::unordered_map<std::type_index, Registration> m_registrations;
    std::unordered_map<std::type_index,
                       std::unordered_map<QString, Registration>> m_namedRegistrations;
    mutable std::recursive_mutex m_mutex;

    // ... 实现细节
};
```

#### 3.1.4 ContainerLocator（全局访问点）

```cpp
class ContainerLocator
{
public:
    static void SetContainerExtension(std::shared_ptr<IContainerExtension> extension);
    static std::shared_ptr<IContainerExtension> Current();
    static std::shared_ptr<IContainerProvider> Container();
    static void ResetContainer();

private:
    static std::shared_ptr<IContainerExtension> s_current;
    static std::mutex s_mutex;
};
```

#### 3.1.5 生命周期管理

| 生命周期 | 说明 | API |
|----------|------|-----|
| Transient | 每次 Resolve 创建新实例 | `Register<TInterface, TImpl>()` |
| Singleton | 全局唯一实例，首次 Resolve 时创建 | `RegisterSingleton<TInterface, TImpl>()` |
| Instance | 外部提供的已有实例 | `RegisterInstance<T>(instance)` |

---

### 3.2 事件聚合器 (`QtPrismEvents`)

#### 3.2.1 设计思路

Prism 的 EventAggregator 是独立于 Qt 信号槽的 **类型化消息总线**，其核心优势是：
- **完全解耦**：发布者和订阅者互不知晓
- **类型安全**：通过事件类型匹配
- **线程调度**：可指定回调在哪个线程执行
- **事件过滤**：订阅时可附加过滤条件

#### 3.2.2 线程选项

```cpp
enum class ThreadOption
{
    PublisherThread,    // 在发布者线程执行回调
    UIThread,          // 通过 QMetaObject::invokeMethod 调度到 UI 线程
    BackgroundThread   // 通过 QtConcurrent 调度到线程池
};
```

#### 3.2.3 核心接口

```cpp
// ==================== EventBase ====================
class EventBase
{
public:
    virtual ~EventBase() = default;
    virtual void Prune() = 0;  // 清理已失效的弱引用订阅
};

// ==================== PubSubEvent<TPayload> ====================
template<typename TPayload>
class PubSubEvent : public EventBase
{
public:
    using Handler = std::function<void(const TPayload&)>;
    using Filter  = std::function<bool(const TPayload&)>;

    struct SubscriptionToken
    {
        uint64_t id;
        void Dispose();  // 取消订阅
    };

    // 完整订阅接口
    SubscriptionToken Subscribe(
        Handler action,
        ThreadOption threadOption = ThreadOption::PublisherThread,
        bool keepSubscriberReferenceAlive = true,
        Filter filter = nullptr
    );

    // 发布事件
    void Publish(const TPayload& payload);

    // 取消订阅
    void Unsubscribe(const SubscriptionToken& token);

    // 清理无效订阅
    void Prune() override;

private:
    struct Subscription
    {
        uint64_t id;
        Handler action;
        Filter filter;
        ThreadOption threadOption;
        std::weak_ptr<void> weakRef;  // 弱引用模式
        bool isStrong;
    };

    std::vector<Subscription> m_subscriptions;
    mutable std::mutex m_mutex;
    static std::atomic<uint64_t> s_nextId;
};

// ==================== 无载荷事件特化 ====================
template<>
class PubSubEvent<void> : public EventBase
{
public:
    using Handler = std::function<void()>;
    SubscriptionToken Subscribe(Handler action,
                                ThreadOption threadOption = ThreadOption::PublisherThread);
    void Publish();
    void Unsubscribe(const SubscriptionToken& token);
};

// ==================== IEventAggregator ====================
class IEventAggregator
{
public:
    virtual ~IEventAggregator() = default;

    template<typename TEvent>
    std::shared_ptr<TEvent> GetEvent();

    virtual std::shared_ptr<EventBase> GetEvent(std::type_index eventType) = 0;
};
```

#### 3.2.4 使用示例

```cpp
// 1. 定义事件
class OrderSubmittedEvent : public PubSubEvent<OrderDetails> {};
class AppShutdownEvent : public PubSubEvent<void> {};

// 2. 订阅
auto ea = container->Resolve<IEventAggregator>();
auto token = ea->GetEvent<OrderSubmittedEvent>()->Subscribe(
    [this](const OrderDetails& order) { handleOrder(order); },
    ThreadOption::UIThread,
    true,
    [](const OrderDetails& order) { return order.total > 1000; }
);

// 3. 发布
ea->GetEvent<OrderSubmittedEvent>()->Publish(orderDetails);

// 4. 取消
token.Dispose();
```

---

### 3.3 模块/插件系统 (`QtPrismCore` + `QtPrismFramework`)

#### 3.3.1 设计思路

结合 Prism 的模块化理念与 Qt 的插件系统：
- **IModule** 定义模块契约
- **Qt Plugin** 作为模块的物理载体（`.so` / `.dll` / `.dylib`）
- **ModuleCatalog** 管理模块元数据
- **ModuleManager** 编排模块加载与初始化生命周期

#### 3.3.2 核心接口

```cpp
// ==================== IModule ====================
class IModule
{
public:
    virtual ~IModule() = default;

    // 注册模块提供的类型和服务到 IoC 容器
    virtual void RegisterTypes(IContainerRegistry* containerRegistry) = 0;

    // 模块初始化完成后调用（订阅事件、注册视图到 Region 等）
    virtual void OnInitialized(IContainerProvider* containerProvider) = 0;
};

// Qt 插件接口声明
#define QtPrism_IModule_iid "org.qtprism.IModule/1.0"
Q_DECLARE_INTERFACE(IModule, QtPrism_IModule_iid)

// ==================== ModuleInfo ====================
struct ModuleInfo
{
    QString moduleName;           // 模块名称
    QString moduleType;           // 类型标识
    QString ref;                  // 插件文件路径或引用
    QStringList dependsOn;        // 依赖的模块名称列表
    InitializationMode initMode;  // 初始化模式
    ModuleState state;            // 当前状态

    enum class InitializationMode
    {
        WhenAvailable,  // 启动时自动加载
        OnDemand        // 按需加载
    };

    enum class ModuleState
    {
        NotStarted,
        ReadyForInitialization,
        Initializing,
        Initialized
    };
};
```

#### 3.3.3 模块目录 (ModuleCatalog)

```cpp
// ==================== IModuleCatalog ====================
class IModuleCatalog
{
public:
    virtual ~IModuleCatalog() = default;

    virtual QList<ModuleInfo> Modules() const = 0;
    virtual void AddModule(const ModuleInfo& moduleInfo) = 0;
    virtual QList<ModuleInfo> GetDependentModules(const ModuleInfo& moduleInfo) const = 0;
    virtual QList<ModuleInfo> CompleteListWithDependencies(
        const QList<ModuleInfo>& modules) const = 0;
    virtual void Initialize() = 0;
};

// ==================== 目录实现 ====================
class ModuleCatalog : public IModuleCatalog { ... };               // 代码注册
class DirectoryModuleCatalog : public IModuleCatalog { ... };      // 目录扫描
class ConfigModuleCatalog : public IModuleCatalog { ... };         // JSON/XML 配置
class AggregateModuleCatalog : public IModuleCatalog { ... };      // 聚合目录
```

#### 3.3.4 模块管理器 (ModuleManager)

```cpp
class IModuleManager : public QObject
{
    Q_OBJECT
public:
    virtual ~IModuleManager() = default;

    virtual void Run() = 0;
    virtual void LoadModule(const QString& moduleName) = 0;

signals:
    void moduleLoaded(const QString& moduleName);
    void moduleLoadFailed(const QString& moduleName, const QString& error);
};

class ModuleManager : public IModuleManager
{
    Q_OBJECT
public:
    ModuleManager(std::shared_ptr<IModuleCatalog> catalog,
                  std::shared_ptr<IContainerExtension> container);

    void Run() override;
    void LoadModule(const QString& moduleName) override;

private:
    void loadModulesWhenAvailable();
    void initializeModule(ModuleInfo& info);
    QList<ModuleInfo> topologicalSort(const QList<ModuleInfo>& modules);
    void validateCatalog();

    std::shared_ptr<IModuleCatalog> m_catalog;
    std::shared_ptr<IContainerExtension> m_container;
    QMap<QString, QPluginLoader*> m_loaders;
    QMap<QString, IModule*> m_loadedModules;
};
```

#### 3.3.5 模块生命周期

```
                 ┌──────────────┐
                 │  NotStarted  │
                 └──────┬───────┘
                        │ ModuleManager.Run()
                        │ 验证依赖、拓扑排序
                        ▼
          ┌──────────────────────────┐
          │  ReadyForInitialization  │
          └──────────┬───────────────┘
                     │ QPluginLoader 加载插件
                     │ qobject_cast<IModule*>
                     ▼
              ┌──────────────┐
              │ Initializing │
              │              │
              │ 1. RegisterTypes(registry)
              │ 2. OnInitialized(provider)
              └──────┬───────┘
                     │
                     ▼
              ┌──────────────┐
              │ Initialized  │
              └──────────────┘
```

#### 3.3.6 模块插件实现示例

```cpp
// ModuleA.h
class ModuleA : public QObject, public IModule
{
    Q_OBJECT
    Q_INTERFACES(IModule)
    Q_PLUGIN_METADATA(IID QtPrism_IModule_iid FILE "module_a.json")

public:
    void RegisterTypes(IContainerRegistry* registry) override
    {
        registry->RegisterSingleton<IOrderService, OrderServiceImpl>();
        registry->Register<IOrderValidator, OrderValidator>();
    }

    void OnInitialized(IContainerProvider* provider) override
    {
        auto regionManager = provider->Resolve<IRegionManager>();
        regionManager->RegisterViewWithRegion("MainRegion", "OrderListView");

        auto ea = provider->Resolve<IEventAggregator>();
        ea->GetEvent<AppStartedEvent>()->Subscribe(
            [this](const auto&) { onAppStarted(); },
            ThreadOption::UIThread
        );
    }
};
```

**`module_a.json`（插件元数据）：**

```json
{
    "moduleName": "ModuleA",
    "version": "1.0.0",
    "dependsOn": [],
    "initializationMode": "WhenAvailable",
    "description": "Order management module"
}
```

---

### 3.4 界面容器系统 — Region (`QtPrismCore` + `QtPrismFramework`)

#### 3.4.1 设计思路

Prism 的 Region 系统将 **UI 容器**（ContentControl, TabControl 等）抽象为命名区域，
实现视图与布局的解耦。在 Qt/QML 中，我们映射为：

| Prism 概念 | Qt/QML 对应 |
|-----------|------------|
| `ContentControl` + `SingleActiveRegion` | QML `Loader` — 一次显示一个视图 |
| `ItemsControl` + `AllActiveRegion` | QML `Column`/`Row`/`Repeater` — 同时显示所有视图 |
| `TabControl` + `Region` | QML `TabBar` + `StackLayout` — 标签切换 |
| `Selector` + `Region` | QML `SwipeView` / `StackView` — 选择性显示 |
| `RegionManager.RegionName` attached property | QML attached property 或自定义 QML 类型 |

#### 3.4.2 核心接口

```cpp
// ==================== IRegion ====================
class IRegion : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name CONSTANT)
    Q_PROPERTY(int viewCount READ viewCount NOTIFY viewsChanged)
    Q_PROPERTY(QObject* activeView READ activeView NOTIFY activeViewChanged)

public:
    virtual ~IRegion() = default;

    virtual QString name() const = 0;
    virtual int viewCount() const = 0;

    // 视图管理
    virtual void Add(QObject* view, const QString& viewName = {}) = 0;
    virtual void Remove(QObject* view) = 0;
    virtual void Activate(QObject* view) = 0;
    virtual void Deactivate(QObject* view) = 0;
    virtual QObject* GetView(const QString& viewName) const = 0;

    // 活动视图
    virtual QObject* activeView() const = 0;
    virtual QList<QObject*> activeViews() const = 0;
    virtual QList<QObject*> views() const = 0;

    // 导航
    virtual void RequestNavigate(const QString& viewName,
                                 const QVariantMap& parameters = {},
                                 std::function<void(bool)> callback = nullptr) = 0;

    // Region 行为
    virtual void AddBehavior(const QString& key,
                             std::shared_ptr<IRegionBehavior> behavior) = 0;

signals:
    void viewsChanged();
    void activeViewChanged();
    void navigating(const QString& viewName);
    void navigated(const QString& viewName);
};

// ==================== IRegionManager ====================
class IRegionManager : public QObject
{
    Q_OBJECT

public:
    virtual ~IRegionManager() = default;

    // Region 管理
    virtual void AddRegion(const QString& regionName,
                           std::shared_ptr<IRegion> region) = 0;
    virtual void RemoveRegion(const QString& regionName) = 0;
    virtual std::shared_ptr<IRegion> GetRegion(const QString& regionName) const = 0;
    virtual bool ContainsRegion(const QString& regionName) const = 0;
    virtual QStringList RegionNames() const = 0;

    // 视图注册（View Discovery 模式）
    virtual void RegisterViewWithRegion(const QString& regionName,
                                        const QString& viewName) = 0;

    // 导航快捷方法
    Q_INVOKABLE virtual void RequestNavigate(const QString& regionName,
                                              const QString& viewName,
                                              const QVariantMap& parameters = {}) = 0;

signals:
    void regionAdded(const QString& regionName);
    void regionRemoved(const QString& regionName);
};
```

#### 3.4.3 Region 适配器

```cpp
// ==================== IRegionAdapter ====================
class IRegionAdapter
{
public:
    virtual ~IRegionAdapter() = default;
    virtual std::shared_ptr<IRegion> Initialize(QObject* regionTarget,
                                                const QString& regionName) = 0;
};

// ==================== 具体适配器 ====================

// 对应 QML Loader — 单活动视图
class LoaderRegionAdapter : public IRegionAdapter
{
public:
    std::shared_ptr<IRegion> Initialize(QObject* regionTarget,
                                        const QString& regionName) override;
    // 创建 SingleActiveRegion，将 Loader.sourceComponent 绑定到活动视图
};

// 对应 QML StackView — 栈式导航
class StackViewRegionAdapter : public IRegionAdapter
{
public:
    std::shared_ptr<IRegion> Initialize(QObject* regionTarget,
                                        const QString& regionName) override;
    // 创建 Region，使用 StackView.push/pop 管理视图
};

// 对应 QML TabBar + StackLayout — 标签页
class TabRegionAdapter : public IRegionAdapter
{
public:
    std::shared_ptr<IRegion> Initialize(QObject* regionTarget,
                                        const QString& regionName) override;
    // 创建 Region，每个视图作为一个标签页
};

// 对应 QML Repeater/Column — 所有视图同时显示
class ItemsRegionAdapter : public IRegionAdapter
{
public:
    std::shared_ptr<IRegion> Initialize(QObject* regionTarget,
                                        const QString& regionName) override;
    // 创建 AllActiveRegion，通过 Repeater model 管理视图
};

// ==================== 适配器映射 ====================
class RegionAdapterMappings
{
public:
    void RegisterMapping(const QString& qmlType,
                         std::shared_ptr<IRegionAdapter> adapter);
    std::shared_ptr<IRegionAdapter> GetAdapter(const QString& qmlType) const;

private:
    QMap<QString, std::shared_ptr<IRegionAdapter>> m_mappings;
};
```

#### 3.4.4 Region 行为

```cpp
class IRegionBehavior
{
public:
    virtual ~IRegionBehavior() = default;
    virtual void SetRegion(IRegion* region) = 0;
    virtual void Attach() = 0;
};

// 自动填充行为 — 将 RegisterViewWithRegion 注册的视图自动添加到 Region
class AutoPopulateRegionBehavior : public IRegionBehavior { ... };

// 活动感知行为 — 设置视图的 IActiveAware::isActive
class RegionActiveAwareBehavior : public IRegionBehavior { ... };

// 成员生命周期行为 — KeepAlive=false 时在 deactivate 后移除视图
class RegionMemberLifetimeBehavior : public IRegionBehavior { ... };

// 销毁行为 — 视图从 Region 移除时调用 IDestructible::destroy()
class DestructibleRegionBehavior : public IRegionBehavior { ... };
```

#### 3.4.5 QML 端 Region 声明

```qml
import QtPrism 1.0

// 方式一：使用 RegionHost 组件
RegionHost {
    regionName: "MainRegion"
    adapterType: "Loader"        // "Loader" | "StackView" | "Tab" | "Items"
    anchors.fill: parent
}

// 方式二：使用 attached property（更灵活）
Loader {
    QtPrism.regionName: "ContentRegion"
    anchors.fill: parent
}

StackView {
    QtPrism.regionName: "NavigationRegion"
    anchors.fill: parent
}

TabBar {
    id: tabBar
    QtPrism.regionName: "TabRegion"
}
```

#### 3.4.6 视图注入 vs 视图发现

```cpp
// View Discovery（声明式，自动填充）
regionManager->RegisterViewWithRegion("MainRegion", "DashboardView");

// View Injection（命令式，手动控制）
auto region = regionManager->GetRegion("SidebarRegion");
auto view = container->Resolve<QObject>("UserProfileView");
region->Add(view, "UserProfile");
region->Activate(view);
```

---

### 3.5 导航系统 (`QtPrismCore` + `QtPrismFramework`)

#### 3.5.1 核心接口

```cpp
// ==================== NavigationContext ====================
class NavigationContext
{
public:
    NavigationContext(const QString& viewName, const QVariantMap& parameters);

    QString viewName() const;
    QVariantMap parameters() const;

    template<typename T>
    T GetParameter(const QString& key, const T& defaultValue = T()) const;
};

// ==================== INavigationAware ====================
// 视图或 ViewModel 实现此接口参与导航生命周期
class INavigationAware
{
public:
    virtual ~INavigationAware() = default;

    // 导航到此视图时调用
    virtual void OnNavigatedTo(const NavigationContext& context) = 0;

    // 从此视图导航离开时调用
    virtual void OnNavigatedFrom(const NavigationContext& context) = 0;

    // 判断当前实例是否可以处理新的导航请求（复用 vs 新建）
    virtual bool IsNavigationTarget(const NavigationContext& context) = 0;
};

// ==================== IConfirmNavigation ====================
// 导航拦截（确认对话框等）
class IConfirmNavigation
{
public:
    virtual ~IConfirmNavigation() = default;

    // 异步确认是否允许导航离开
    virtual void ConfirmNavigationRequest(const NavigationContext& context,
                                          std::function<void(bool)> callback) = 0;
};

// ==================== IDestructible ====================
// 视图销毁钩子
class IDestructible
{
public:
    virtual ~IDestructible() = default;
    virtual void Destroy() = 0;
};

// ==================== IActiveAware ====================
class IActiveAware
{
public:
    virtual ~IActiveAware() = default;
    virtual bool isActive() const = 0;
    virtual void setIsActive(bool active) = 0;
};
```

#### 3.5.2 导航日志 (Navigation Journal)

```cpp
class INavigationJournal
{
public:
    virtual ~INavigationJournal() = default;

    virtual bool CanGoBack() const = 0;
    virtual bool CanGoForward() const = 0;
    virtual void GoBack() = 0;
    virtual void GoForward() = 0;
    virtual void RecordNavigation(const QString& viewName,
                                  const QVariantMap& parameters) = 0;
    virtual void Clear() = 0;
};

class NavigationJournal : public INavigationJournal
{
public:
    // 使用双向链表实现前进/后退
    // GoBack/GoForward 触发 Region 的 RequestNavigate

private:
    struct JournalEntry
    {
        QString viewName;
        QVariantMap parameters;
    };

    QList<JournalEntry> m_backStack;
    QList<JournalEntry> m_forwardStack;
    std::optional<JournalEntry> m_currentEntry;
    IRegion* m_region = nullptr;
};
```

#### 3.5.3 导航流程

```
RequestNavigate("MainRegion", "OrderDetailView", {{"orderId", 42}})
    │
    ▼
┌──────────────────────────────────────┐
│ 1. 获取 Region("MainRegion")          │
└──────┬───────────────────────────────┘
       │
       ▼
┌──────────────────────────────────────┐
│ 2. 对当前活动视图检查                    │
│    IConfirmNavigation?                │
│    → ConfirmNavigationRequest(ctx, cb)│
│    → 若 cb(false) → 取消导航           │
└──────┬───────────────────────────────┘
       │ cb(true)
       ▼
┌──────────────────────────────────────┐
│ 3. 当前视图 OnNavigatedFrom(ctx)       │
└──────┬───────────────────────────────┘
       │
       ▼
┌──────────────────────────────────────┐
│ 4. 查找目标视图                         │
│    → 遍历 Region 中已有视图             │
│    → IsNavigationTarget(ctx)?         │
│    → 匹配则复用，否则从容器创建新实例      │
└──────┬───────────────────────────────┘
       │
       ▼
┌──────────────────────────────────────┐
│ 5. Region.Activate(targetView)        │
│    → 更新 QML UI                      │
└──────┬───────────────────────────────┘
       │
       ▼
┌──────────────────────────────────────┐
│ 6. Journal.RecordNavigation(...)      │
│    → 清空 forwardStack                │
└──────┬───────────────────────────────┘
       │
       ▼
┌──────────────────────────────────────┐
│ 7. 目标视图 OnNavigatedTo(ctx)         │
│    → 传递参数 {orderId: 42}            │
└──────────────────────────────────────┘
```

---

### 3.6 服务与服务接口设计

#### 3.6.1 框架内置服务注册表

```cpp
void QtPrismApplication::RegisterRequiredTypes(IContainerRegistry* registry)
{
    // 核心服务 — 单例
    registry->RegisterSingleton<IEventAggregator, EventAggregator>();
    registry->RegisterSingleton<IRegionManager, RegionManager>();
    registry->RegisterSingleton<IModuleManager, ModuleManager>();
    registry->RegisterSingleton<IModuleCatalog, ModuleCatalog>();
    registry->RegisterSingleton<IRegionViewRegistry, RegionViewRegistry>();
    registry->RegisterSingleton<IRegionBehaviorFactory, RegionBehaviorFactory>();
    registry->RegisterSingleton<RegionAdapterMappings, RegionAdapterMappings>();

    // 按需创建的服务 — 瞬态
    registry->Register<INavigationJournal, NavigationJournal>();
    registry->Register<IRegionNavigationService, RegionNavigationService>();
}
```

#### 3.6.2 服务接口设计原则

1. **接口隔离**：每个服务定义独立的抽象接口（纯虚类）
2. **依赖倒置**：高层模块不直接依赖低层实现，通过接口交互
3. **构造注入**：服务通过构造函数接收依赖

```cpp
// 服务接口示例
class IOrderService
{
public:
    virtual ~IOrderService() = default;
    virtual QList<Order> GetOrders() const = 0;
    virtual Order GetOrderById(int id) const = 0;
    virtual void SubmitOrder(const Order& order) = 0;
};

class IUserService
{
public:
    virtual ~IUserService() = default;
    virtual User GetCurrentUser() const = 0;
    virtual bool Authenticate(const QString& username, const QString& password) = 0;
};

// 模块中注册
void OrderModule::RegisterTypes(IContainerRegistry* registry)
{
    registry->RegisterSingleton<IOrderService, OrderServiceImpl>();
}

// 其他模块中使用
void DashboardModule::OnInitialized(IContainerProvider* provider)
{
    auto orderService = provider->Resolve<IOrderService>();
    // 使用 orderService...
}
```

#### 3.6.3 视图注册与解析

```cpp
// 注册视图（QML 组件）用于导航
registry->RegisterForNavigation("OrderListView", QUrl("qrc:/modules/order/OrderListView.qml"));
registry->RegisterForNavigation("OrderDetailView", QUrl("qrc:/modules/order/OrderDetailView.qml"));

// IViewRegistry 管理视图名到 QML 组件 URL 的映射
class IViewRegistry
{
public:
    virtual void RegisterView(const QString& viewName, const QUrl& qmlUrl) = 0;
    virtual QUrl GetViewUrl(const QString& viewName) const = 0;
    virtual QObject* CreateView(const QString& viewName, QObject* parent = nullptr) = 0;
};
```

---

### 3.7 属性系统（Qt 适配）

#### 3.7.1 对照映射

Prism 的 `BindableBase` 提供 `INotifyPropertyChanged` 实现，在 Qt 中有**原生对应**：

| Prism (.NET) | Qt/QML (C++) |
|-------------|-------------|
| `BindableBase` | `QObject` |
| `INotifyPropertyChanged` | Qt 信号机制 |
| `SetProperty(ref field, value)` | 属性 setter + `emit signal()` |
| `PropertyChanged` 事件 | `Q_PROPERTY` 的 `NOTIFY` 信号 |
| `[CallerMemberName]` | `Q_PROPERTY` 宏自动处理 |
| `ObservesProperty()` | Qt 属性绑定 / `QML Binding` |

#### 3.7.2 推荐的 ViewModel 基类

```cpp
class ViewModelBase : public QObject
{
    Q_OBJECT

public:
    explicit ViewModelBase(QObject* parent = nullptr) : QObject(parent) {}

protected:
    // 辅助宏：简化属性定义
    // 自动处理值比较和信号发射
    template<typename T>
    bool setProperty(T& storage, const T& value, void (ViewModelBase::*signal)())
    {
        if (storage == value)
            return false;
        storage = value;
        (this->*signal)();
        return true;
    }
};

// 便捷宏
#define QTPRISM_PROPERTY(type, name, notifySignal)                        \
    Q_PROPERTY(type name READ name WRITE set##name NOTIFY notifySignal)  \
public:                                                                  \
    type name() const { return m_##name; }                               \
    void set##name(const type& value)                                    \
    {                                                                    \
        if (m_##name != value)                                           \
        {                                                                \
            m_##name = value;                                            \
            emit notifySignal();                                         \
        }                                                                \
    }                                                                    \
Q_SIGNALS:                                                               \
    void notifySignal();                                                 \
private:                                                                 \
    type m_##name;

// 使用示例
class OrderViewModel : public ViewModelBase, public INavigationAware
{
    Q_OBJECT

    QTPRISM_PROPERTY(QString, orderTitle, orderTitleChanged)
    QTPRISM_PROPERTY(double, totalAmount, totalAmountChanged)
    QTPRISM_PROPERTY(bool, isLoading, isLoadingChanged)

public:
    explicit OrderViewModel(std::shared_ptr<IOrderService> orderService,
                            QObject* parent = nullptr);

    void OnNavigatedTo(const NavigationContext& context) override;
    void OnNavigatedFrom(const NavigationContext& context) override;
    bool IsNavigationTarget(const NavigationContext& context) override;

private:
    std::shared_ptr<IOrderService> m_orderService;
};
```

---

## 四、样例工程设计

### 4.1 应用场景

构建一个 **模块化后台管理系统**，包含以下功能模块：

| 模块名 | 功能 | 加载方式 |
|--------|------|---------|
| `ShellModule` | 主窗口框架（侧边栏、标题栏、状态栏） | 内置 |
| `DashboardModule` | 仪表盘/首页 | WhenAvailable |
| `OrderModule` | 订单管理（列表、详情、导航） | WhenAvailable |
| `SettingsModule` | 系统设置 | OnDemand |

### 4.2 界面布局

```
┌────────────────────────────────────────────────────────┐
│                    TitleBar Region                      │
├──────────┬─────────────────────────────────────────────┤
│          │                                             │
│ Sidebar  │              Main Region                    │
│ Region   │         (Loader / StackView)                │
│          │                                             │
│ ┌──────┐ │  ┌─────────────────────────────────────┐   │
│ │ Nav  │ │  │                                     │   │
│ │ Item │ │  │         Content Area                 │   │
│ │      │ │  │    (Region-based navigation)         │   │
│ │ ...  │ │  │                                     │   │
│ │      │ │  │                                     │   │
│ └──────┘ │  └─────────────────────────────────────┘   │
│          │                                             │
├──────────┴─────────────────────────────────────────────┤
│                   StatusBar Region                      │
└────────────────────────────────────────────────────────┘
```

### 4.3 目录结构

```
QtPrism/
├── CMakeLists.txt                          # 顶层 CMake
├── QtPrism_Architecture_Design.md          # 本文档
│
├── src/
│   ├── QtPrismContainer/                   # IoC 容器库
│   │   ├── CMakeLists.txt
│   │   ├── IContainerRegistry.h
│   │   ├── IContainerProvider.h
│   │   ├── IContainerExtension.h
│   │   ├── ContainerLocator.h
│   │   ├── ContainerLocator.cpp
│   │   ├── QtPrismContainerImpl.h
│   │   └── QtPrismContainerImpl.cpp
│   │
│   ├── QtPrismEvents/                      # 事件聚合器库
│   │   ├── CMakeLists.txt
│   │   ├── EventBase.h
│   │   ├── PubSubEvent.h
│   │   ├── IEventAggregator.h
│   │   ├── EventAggregator.h
│   │   └── EventAggregator.cpp
│   │
│   ├── QtPrismCore/                        # 核心接口库
│   │   ├── CMakeLists.txt
│   │   ├── Modularity/
│   │   │   ├── IModule.h
│   │   │   ├── ModuleInfo.h
│   │   │   ├── IModuleCatalog.h
│   │   │   └── IModuleManager.h
│   │   ├── Regions/
│   │   │   ├── IRegion.h
│   │   │   ├── IRegionManager.h
│   │   │   ├── IRegionAdapter.h
│   │   │   ├── IRegionBehavior.h
│   │   │   ├── IRegionBehaviorFactory.h
│   │   │   └── IViewRegistry.h
│   │   ├── Navigation/
│   │   │   ├── NavigationContext.h
│   │   │   ├── INavigationAware.h
│   │   │   ├── IConfirmNavigation.h
│   │   │   ├── INavigationJournal.h
│   │   │   └── IDestructible.h
│   │   ├── Services/
│   │   │   └── IActiveAware.h
│   │   └── Mvvm/
│   │       ├── ViewModelBase.h
│   │       └── QtPrismProperty.h           # QTPRISM_PROPERTY 宏
│   │
│   └── QtPrismFramework/                   # Qt/QML 平台实现库
│       ├── CMakeLists.txt
│       ├── Modularity/
│       │   ├── ModuleCatalog.h / .cpp
│       │   ├── DirectoryModuleCatalog.h / .cpp
│       │   ├── ConfigModuleCatalog.h / .cpp
│       │   ├── AggregateModuleCatalog.h / .cpp
│       │   └── ModuleManager.h / .cpp
│       ├── Regions/
│       │   ├── Region.h / .cpp                 # 默认 Region 实现
│       │   ├── SingleActiveRegion.h / .cpp
│       │   ├── AllActiveRegion.h / .cpp
│       │   ├── RegionManager.h / .cpp
│       │   ├── RegionAdapterMappings.h / .cpp
│       │   ├── Adapters/
│       │   │   ├── LoaderRegionAdapter.h / .cpp
│       │   │   ├── StackViewRegionAdapter.h / .cpp
│       │   │   ├── TabRegionAdapter.h / .cpp
│       │   │   └── ItemsRegionAdapter.h / .cpp
│       │   ├── Behaviors/
│       │   │   ├── AutoPopulateRegionBehavior.h / .cpp
│       │   │   ├── RegionActiveAwareBehavior.h / .cpp
│       │   │   ├── RegionMemberLifetimeBehavior.h / .cpp
│       │   │   └── DestructibleRegionBehavior.h / .cpp
│       │   └── RegionBehaviorFactory.h / .cpp
│       ├── Navigation/
│       │   ├── RegionNavigationService.h / .cpp
│       │   └── NavigationJournal.h / .cpp
│       ├── Views/
│       │   ├── ViewRegistry.h / .cpp
│       │   └── RegionHost.h / .cpp             # QML RegionHost 组件
│       ├── Qml/
│       │   ├── RegionHost.qml
│       │   └── qmldir
│       └── QtPrismApplication.h / .cpp         # 应用启动引导类
│
├── samples/
│   └── AdminApp/                               # 样例应用
│       ├── CMakeLists.txt
│       ├── main.cpp
│       ├── App.h / .cpp                        # 继承 QtPrismApplication
│       ├── Shell/
│       │   ├── ShellView.qml                   # 主窗口布局
│       │   └── ShellViewModel.h / .cpp
│       ├── Modules/
│       │   ├── DashboardModule/
│       │   │   ├── CMakeLists.txt
│       │   │   ├── DashboardModule.h / .cpp
│       │   │   ├── module_dashboard.json
│       │   │   ├── ViewModels/
│       │   │   │   └── DashboardViewModel.h / .cpp
│       │   │   ├── Views/
│       │   │   │   └── DashboardView.qml
│       │   │   └── resources.qrc
│       │   ├── OrderModule/
│       │   │   ├── CMakeLists.txt
│       │   │   ├── OrderModule.h / .cpp
│       │   │   ├── module_order.json
│       │   │   ├── Services/
│       │   │   │   ├── IOrderService.h
│       │   │   │   └── OrderServiceImpl.h / .cpp
│       │   │   ├── ViewModels/
│       │   │   │   ├── OrderListViewModel.h / .cpp
│       │   │   │   └── OrderDetailViewModel.h / .cpp
│       │   │   ├── Views/
│       │   │   │   ├── OrderListView.qml
│       │   │   │   └── OrderDetailView.qml
│       │   │   ├── Events/
│       │   │   │   └── OrderEvents.h
│       │   │   └── resources.qrc
│       │   └── SettingsModule/
│       │       ├── CMakeLists.txt
│       │       ├── SettingsModule.h / .cpp
│       │       ├── module_settings.json
│       │       ├── ViewModels/
│       │       │   └── SettingsViewModel.h / .cpp
│       │       ├── Views/
│       │       │   └── SettingsView.qml
│       │       └── resources.qrc
│       └── resources/
│           └── main.qrc
│
└── tests/                                      # 单元测试
    ├── CMakeLists.txt
    ├── ContainerTests.cpp
    ├── EventAggregatorTests.cpp
    ├── ModuleManagerTests.cpp
    ├── RegionManagerTests.cpp
    └── NavigationTests.cpp
```

---

## 五、应用启动引导流程

### 5.1 QtPrismApplication 基类

```cpp
class QtPrismApplication : public QObject
{
    Q_OBJECT

public:
    explicit QtPrismApplication(QGuiApplication* app, QQmlApplicationEngine* engine);

    void Initialize();

protected:
    // 可重写的初始化钩子（对应 Prism 的 Template Method 模式）

    // Step 1: 创建 IoC 容器
    virtual std::shared_ptr<IContainerExtension> CreateContainerExtension();

    // Step 2: 创建模块目录
    virtual std::shared_ptr<IModuleCatalog> CreateModuleCatalog();

    // Step 3: 注册框架必需类型
    virtual void RegisterRequiredTypes(IContainerRegistry* registry);

    // Step 4: 用户注册自定义类型
    virtual void RegisterTypes(IContainerRegistry* registry) = 0;

    // Step 5: 配置模块目录
    virtual void ConfigureModuleCatalog(IModuleCatalog* catalog) = 0;

    // Step 6: 配置 Region 适配器映射
    virtual void ConfigureRegionAdapterMappings(RegionAdapterMappings* mappings);

    // Step 7: 配置默认 Region 行为
    virtual void ConfigureDefaultRegionBehaviors(IRegionBehaviorFactory* factory);

    // Step 8: 创建 Shell（主窗口）
    virtual QUrl CreateShell() = 0;

    // Step 9: 初始化 Shell 后回调
    virtual void OnInitialized();

private:
    void InitializeModules();
    void RegisterQmlTypes();

    QGuiApplication* m_app;
    QQmlApplicationEngine* m_engine;
    std::shared_ptr<IContainerExtension> m_container;
};
```

### 5.2 完整启动序列

```
main()
  → QGuiApplication app
  → QQmlApplicationEngine engine
  → AdminApp prismApp(&app, &engine)     // 继承 QtPrismApplication
  → prismApp.Initialize()
      │
      ├─ 1. CreateContainerExtension()
      │     → 创建 QtPrismContainerImpl
      │     → ContainerLocator::SetContainerExtension()
      │
      ├─ 2. CreateModuleCatalog()
      │     → 创建 DirectoryModuleCatalog 或 ModuleCatalog
      │
      ├─ 3. RegisterRequiredTypes()
      │     → 注册 IEventAggregator, IRegionManager, IModuleManager...
      │
      ├─ 4. RegisterTypes()                [用户重写]
      │     → 注册应用级服务
      │
      ├─ 5. ConfigureModuleCatalog()       [用户重写]
      │     → 添加模块到目录
      │
      ├─ 6. ConfigureRegionAdapterMappings()
      │     → 注册 Loader/StackView/Tab/Items 适配器
      │
      ├─ 7. ConfigureDefaultRegionBehaviors()
      │     → 注册 AutoPopulate, ActiveAware, Lifetime, Destructible
      │
      ├─ 8. RegisterQmlTypes()
      │     → 注册 RegionHost, RegionManager 等到 QML
      │
      ├─ 9. CreateShell()                  [用户重写]
      │     → 返回 Shell QML 的 URL
      │     → engine.load(shellUrl)
      │
      ├─ 10. InitializeModules()
      │      → ModuleManager::Run()
      │         → Catalog.Initialize() → 验证 → 排序
      │         → 逐个加载插件 (QPluginLoader)
      │         → module->RegisterTypes(registry)
      │         → module->OnInitialized(provider)
      │
      └─ 11. OnInitialized()              [用户重写]
             → 应用启动完成，可执行首次导航
```

---

## 六、关键设计决策与 Qt 适配

### 6.1 Prism vs Qt 概念映射总表

| Prism (.NET/WPF) | QtPrism (C++/Qt/QML) | 说明 |
|-------------------|----------------------|------|
| Assembly/DLL | Qt Plugin (.so/.dll) | 模块的物理载体 |
| `IContainerExtension` | `IContainerExtension` | 自实现轻量容器 |
| `IModule` | `IModule` + `Q_INTERFACES` | Qt 插件接口 |
| `ModuleCatalog` | `ModuleCatalog` | 模块元数据管理 |
| `DirectoryModuleCatalog` | `DirectoryModuleCatalog` | 扫描目录发现插件 |
| `RegionManager.RegionName` (attached prop) | `QtPrism.regionName` (QML attached) | 声明式 Region 注册 |
| `ContentControl` Region | `Loader` Region | 单活动视图 |
| `ItemsControl` Region | `Repeater`/`Column` Region | 多视图同时显示 |
| `TabControl` Region | `TabBar`+`StackLayout` Region | 标签页切换 |
| `Selector` Region | `StackView` Region | 导航栈 |
| `RegionAdapter` | `IRegionAdapter` | 适配不同 QML 容器 |
| `RegionBehavior` | `IRegionBehavior` | 可插拔 Region 行为 |
| `IEventAggregator` | `IEventAggregator` | 类型化消息总线 |
| `PubSubEvent<T>` | `PubSubEvent<T>` | 泛型事件 |
| `ThreadOption.UIThread` | `QMetaObject::invokeMethod` | 回到 UI 线程 |
| `ThreadOption.BackgroundThread` | `QtConcurrent::run` | 后台线程 |
| `INavigationAware` | `INavigationAware` | 导航生命周期 |
| `IRegionNavigationJournal` | `INavigationJournal` | 前进/后退 |
| `BindableBase` | `QObject` + `Q_PROPERTY` | **Qt 原生替代** |
| `SetProperty(ref, value)` | `QTPRISM_PROPERTY` 宏 | 简化属性定义 |
| `DelegateCommand` | **不实现** | Qt 已有信号槽 |
| `IDialogService` | **不实现** | Qt 已有 Dialog 方案 |
| `ViewModelLocator` | QML `context property` / `qmlRegisterType` | ViewModel 自动关联 |

### 6.2 关键技术方案

#### 6.2.1 无运行时反射的类型解析

```cpp
// 使用 std::type_index 作为类型键
template<typename T>
std::shared_ptr<T> IContainerProvider::Resolve()
{
    auto ptr = Resolve(std::type_index(typeid(T)));
    return std::static_pointer_cast<T>(ptr);
}

// 注册时捕获工厂函数
template<typename TInterface, typename TImpl>
void IContainerRegistry::Register()
{
    RegisterFactory(
        std::type_index(typeid(TInterface)),
        []() -> std::shared_ptr<void> { return std::make_shared<TImpl>(); }
    );
}
```

#### 6.2.2 QML 与 C++ Region 交互

```cpp
// C++ 端：RegionHost 是一个 QQuickItem
class RegionHost : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(QString regionName READ regionName WRITE setRegionName NOTIFY regionNameChanged)
    Q_PROPERTY(QString adapterType READ adapterType WRITE setAdapterType NOTIFY adapterTypeChanged)
    QML_ELEMENT

public:
    void componentComplete() override
    {
        QQuickItem::componentComplete();
        registerWithRegionManager();
    }

private:
    void registerWithRegionManager()
    {
        auto regionManager = ContainerLocator::Container()->Resolve<IRegionManager>();
        auto adapterMappings = ContainerLocator::Container()
                                   ->Resolve<RegionAdapterMappings>();
        auto adapter = adapterMappings->GetAdapter(m_adapterType);
        auto region = adapter->Initialize(this, m_regionName);
        regionManager->AddRegion(m_regionName, region);
    }
};
```

#### 6.2.3 模块中 QML 视图的加载

```cpp
// 通过 QML 组件 URL 动态创建视图
QObject* ViewRegistry::CreateView(const QString& viewName, QObject* parent)
{
    QUrl url = GetViewUrl(viewName);
    QQmlComponent component(m_engine, url);

    if (component.status() != QQmlComponent::Ready)
    {
        qWarning() << "Failed to load view:" << viewName << component.errorString();
        return nullptr;
    }

    QObject* view = component.create();
    if (view && parent)
    {
        view->setParent(parent);
    }

    return view;
}
```

---

## 七、实施计划

### 7.1 阶段划分

| 阶段 | 内容 | 预估工时 | 交付物 |
|------|------|---------|--------|
| **Phase 1** | IoC 容器 + ContainerLocator | 3-4 天 | `QtPrismContainer` 库 + 单元测试 |
| **Phase 2** | EventAggregator 消息机制 | 2-3 天 | `QtPrismEvents` 库 + 单元测试 |
| **Phase 3** | 核心接口定义 | 2-3 天 | `QtPrismCore` 库（纯头文件接口） |
| **Phase 4** | 模块/插件系统 | 4-5 天 | ModuleCatalog, ModuleManager + 测试 |
| **Phase 5** | Region 系统 | 5-7 天 | Region, Adapters, Behaviors + QML 组件 |
| **Phase 6** | 导航系统 | 3-4 天 | NavigationService, Journal + 测试 |
| **Phase 7** | 应用引导框架 | 2-3 天 | QtPrismApplication + QML 类型注册 |
| **Phase 8** | 样例应用 | 5-7 天 | AdminApp + 3 个功能模块 |
| **Phase 9** | 集成测试与文档 | 3-4 天 | 完整测试 + API 文档 |

**总计：约 29-40 个工作日**

### 7.2 详细里程碑

#### Phase 1：IoC 容器（第 1-4 天）

- [ ] 实现 `IContainerRegistry` / `IContainerProvider` / `IContainerExtension` 接口
- [ ] 实现 `QtPrismContainerImpl` 默认容器
  - [ ] Transient 注册与解析
  - [ ] Singleton 注册与解析（线程安全懒初始化）
  - [ ] Instance 注册
  - [ ] Named 注册与解析
  - [ ] 类型存在性检查
- [ ] 实现 `ContainerLocator` 全局访问点
- [ ] 编写单元测试（覆盖所有生命周期场景）

#### Phase 2：EventAggregator（第 5-7 天）

- [ ] 实现 `EventBase` 基类
- [ ] 实现 `PubSubEvent<T>` 模板类
  - [ ] 订阅（带过滤、线程选项）
  - [ ] 发布（遍历订阅、执行过滤、线程调度）
  - [ ] 取消订阅（Token 和 Handler 两种方式）
  - [ ] 弱引用自动清理
- [ ] 实现 `PubSubEvent<void>` 无载荷特化
- [ ] 实现 `EventAggregator` 单例管理器
- [ ] 实现线程调度（UIThread → `QMetaObject::invokeMethod`，BackgroundThread → `QtConcurrent`）
- [ ] 编写单元测试

#### Phase 3：核心接口（第 8-10 天）

- [ ] 定义 `IModule` 接口 + `Q_DECLARE_INTERFACE`
- [ ] 定义 `ModuleInfo` 数据结构
- [ ] 定义 `IModuleCatalog` / `IModuleManager` 接口
- [ ] 定义 `IRegion` / `IRegionManager` / `IRegionAdapter` / `IRegionBehavior` 接口
- [ ] 定义 `INavigationAware` / `IConfirmNavigation` / `INavigationJournal` 接口
- [ ] 定义 `IActiveAware` / `IDestructible` 接口
- [ ] 定义 `IViewRegistry` 接口
- [ ] 实现 `ViewModelBase` + `QTPRISM_PROPERTY` 宏
- [ ] 实现 `NavigationContext` 值类型

#### Phase 4：模块/插件系统（第 11-15 天）

- [ ] 实现 `ModuleCatalog`（代码注册）
  - [ ] AddModule / Modules / GetDependentModules
  - [ ] 验证逻辑（唯一性、循环依赖检测）
  - [ ] 拓扑排序（Kahn 算法）
- [ ] 实现 `DirectoryModuleCatalog`
  - [ ] 扫描指定目录的 `.so/.dll` 文件
  - [ ] 读取 Qt 插件元数据（JSON）
  - [ ] 自动构建 ModuleInfo
- [ ] 实现 `ConfigModuleCatalog`（JSON 配置文件）
- [ ] 实现 `AggregateModuleCatalog`
- [ ] 实现 `ModuleManager`
  - [ ] Run() 主流程
  - [ ] 按序加载与初始化
  - [ ] OnDemand 模块延迟加载
  - [ ] 错误处理与信号通知
- [ ] 编写单元测试

#### Phase 5：Region 系统（第 16-22 天）

- [ ] 实现 `Region` / `SingleActiveRegion` / `AllActiveRegion`
  - [ ] 视图集合管理
  - [ ] 活动视图管理
  - [ ] 行为集合
- [ ] 实现 `RegionManager`
  - [ ] Region 注册/移除/查找
  - [ ] RegisterViewWithRegion
  - [ ] RequestNavigate 快捷方法
- [ ] 实现 Region 适配器
  - [ ] `LoaderRegionAdapter`（QML Loader）
  - [ ] `StackViewRegionAdapter`（QML StackView）
  - [ ] `TabRegionAdapter`（QML TabBar + StackLayout）
  - [ ] `ItemsRegionAdapter`（QML Repeater）
- [ ] 实现 `RegionAdapterMappings`
- [ ] 实现 Region 行为
  - [ ] `AutoPopulateRegionBehavior`
  - [ ] `RegionActiveAwareBehavior`
  - [ ] `RegionMemberLifetimeBehavior`
  - [ ] `DestructibleRegionBehavior`
- [ ] 实现 `RegionBehaviorFactory`
- [ ] 实现 QML `RegionHost` 组件
- [ ] 实现 `IViewRegistry` / `ViewRegistry`
- [ ] 编写单元测试

#### Phase 6：导航系统（第 23-26 天）

- [ ] 实现 `RegionNavigationService`
  - [ ] 完整导航流程（确认→离开→加载→激活→记录→到达）
  - [ ] 视图复用逻辑（IsNavigationTarget）
  - [ ] 导航参数传递
- [ ] 实现 `NavigationJournal`
  - [ ] Back/Forward 栈管理
  - [ ] RecordNavigation
  - [ ] GoBack / GoForward
- [ ] 编写单元测试

#### Phase 7：应用引导框架（第 27-29 天）

- [ ] 实现 `QtPrismApplication`
  - [ ] 完整 Template Method 初始化序列
  - [ ] 框架类型自动注册
  - [ ] QML 类型注册（`qmlRegisterType` / `QML_ELEMENT`）
- [ ] QML 模块定义（`qmldir`）

#### Phase 8：样例应用（第 30-36 天）

- [ ] 搭建 `AdminApp` 主项目结构
- [ ] 实现 `ShellView.qml`（主窗口布局 + Region 声明）
- [ ] 实现 `DashboardModule`
  - [ ] DashboardView.qml + DashboardViewModel
  - [ ] 展示 EventAggregator 使用
- [ ] 实现 `OrderModule`
  - [ ] OrderListView → OrderDetailView 导航
  - [ ] IOrderService 服务接口
  - [ ] 导航参数传递
  - [ ] 导航日志（前进/后退）
  - [ ] 展示跨模块事件通信
- [ ] 实现 `SettingsModule`（OnDemand 加载示例）
- [ ] 集成测试所有功能

#### Phase 9：测试与文档（第 37-40 天）

- [ ] 容器测试（注册、解析、生命周期、线程安全）
- [ ] EventAggregator 测试（订阅、发布、过滤、线程、弱引用）
- [ ] ModuleManager 测试（加载顺序、依赖、OnDemand）
- [ ] Region 测试（适配器、行为、视图管理）
- [ ] Navigation 测试（完整流程、日志、确认拦截）
- [ ] API 文档注释

---

## 八、技术风险与对策

| 风险 | 影响 | 对策 |
|------|------|------|
| C++ 无运行时反射，类型擦除复杂 | 容器实现难度高 | 使用 `std::type_index` + 模板接口，编译期保证类型安全 |
| QML 动态创建组件性能 | Region 视图切换卡顿 | 视图缓存池 + `Loader.asynchronous` 异步加载 |
| Qt 插件跨平台兼容性 | 模块加载失败 | 统一 CMake 编译配置，插件元数据标准化 |
| QML 与 C++ 数据交互 | 属性绑定断裂 | 严格使用 `Q_PROPERTY` + `NOTIFY` 信号 |
| 模块间循环依赖 | 初始化死锁 | ModuleCatalog 验证阶段检测环路 |
| 弱引用事件订阅 | C++ 弱引用语义复杂 | 提供 `shared_ptr` + `weak_ptr` 方案，默认强引用简化使用 |
| 线程安全 | 容器/事件并发访问 | 使用 `std::mutex` / `std::recursive_mutex` 保护关键区域 |

---

## 九、总结

本方案将 Prism 框架的核心理念完整移植到 C++/Qt/QML 技术栈：

1. **IoC 容器** — 轻量级模板化依赖注入，支持 Transient/Singleton/Instance 生命周期
2. **模块系统** — 基于 Qt Plugin 的可插拔模块，支持依赖排序和按需加载
3. **Region 系统** — QML 容器抽象，支持 Loader/StackView/Tab/Items 四种适配器
4. **EventAggregator** — 类型化松耦合消息总线，支持线程调度和事件过滤
5. **导航系统** — 基于 Region 的视图导航，支持参数传递、生命周期回调和前进/后退
6. **服务设计** — 接口驱动的服务注册/解析，模块间通过接口通信
7. **属性系统** — 利用 Qt 原生 `Q_PROPERTY` 和便捷宏，替代 Prism 的 BindableBase

排除的内容（Qt 已有更好的原生方案）：
- Command → Qt 信号槽 + QML 事件处理
- BindableBase → QObject + Q_PROPERTY
- Dialogs → Qt 原生 Dialog 组件
