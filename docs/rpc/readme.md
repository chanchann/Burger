## RPC

## 集群和分布式

### 单机服务器的局限性

1. 受限于硬件资源，服务器所能承受的用户的并发量有限

2. 任意模块的修改，都会导致整个项目的重新编译(n+ h)，部署(运维复杂)，非常麻烦

3. 系统中，有些模块属于CPU密集型(部署到CPU好的机器上)，有些模块是属于IO密集型, 造成各模块对于硬件资源的需求不一样

### 集群

每一台服务器独立运行一个工程的所有模块。

优点: 用户的并发量提升了（水平拓展）， 简单

缺点: 项目代码还是需要整体重新编译，而且需要多次部署

有些模块并不需要高并发

### 分布式

一个工程拆分了很多模块 每一个模块独立部署运行在一个服务器主机上

每个节点可做集群。

## 软件模块

1. 大系统的软件模块怎么分

各模块可能会实现大量重复的代码

2. 各模块之间应该怎么访问

各个模块都运行在不同的进程里面 or docker 虚拟环境中

机器1上的模块怎么调用机器2上的模块的一个业务方法呢(涉及网络传输)

我们的分布式框架就是要去隐藏这些细节，像是在本机一样调用函数一样

## RPC 通信原理 (分布式通信)

```
      // server1                                   // server2
local call ---> 序列化  --> RPC通信(Burger) -->     反序列化   --> call 
                                                                 |  
local ret  <-- 反序列化<--  RPC通信(Burger) <--      序列化    <-- return


// 序列化/反序列化 : protobuf 
// 网络部分，包括寻找rpc服务主机，发起rpc调用请求和相应rpc调用结果，使用Burger网络库和zookeeper 服务配置中心
```

## protobuf

### protobuf 资料

https://colobu.com/2015/01/07/Protobuf-language-guide/

### 相对于json好处

1. 二进制存储，xml(20倍)和json(10倍)是文本存储

2. protobuf不需要存储额外的信息

json存储 ： name: "MITSK", pwd : "123"

"zhang san""123456"

- protobuf没有提供任何rpc功能，只是做序列化和反序列化

## protobuf rpc 流程剖析

```cpp
// Closure 是个抽象类
class LIBPROTOBUF_EXPORT Closure {
 public:
  Closure() {}
  virtual ~Closure();

  virtual void Run() = 0;

 private:
  GOOGLE_DISALLOW_EVIL_CONSTRUCTORS(Closure);
};
```

## rpc发布服务

```cpp
// Service* 是个基类指针

// RpcProvider的使用者，rpc服务方法的发布方

class UserService : public UserServiceRpc {
      login() <== 本地方法
      
      login(controller, request, response, done) <== 重写protobuf提供的virtual虚函数
      {
            // 网络先接收
            1. 从LoginRequest 获取参数的值
            2. 执行本地服务login, 并获取返回值
            3. 用上面的返回值填写LoginResponse
            4. 一个回调，把LoginResponse发送给rpc client
            // 交给网络发送
      }
}

// 问题 : 接收一个rpc调用请求时，它怎么知道要调用应用程序的哪个服务对象的哪个rpc方法呢？
// 比如UserService的Login方法
// 所以我们NotifyService 需要生成一张表，记录服务对象和其发布的所有服务方法
// UserService Login Register 
// FreindService AddFriend DelFriend GetFriendList
// 这些都继承Service类(描述对象)
// Method类(描述方法)
```

```cpp
// 网络(收发)功能有Burger库实现
// protobuf 实现数据的序列化和反序列化
RpcProvider provider;
provider.NotifyService(new UserService());
provider.Run();
```


```cpp
burgerRpc::UserServiceRpc_Stub stub(&rpcChannel);  

stub.Login(nullptr, &request, &response, nullptr); 
    
--> 其实是 rpcChannel->callMethod(xxx);


```

```cpp
class RpcChannel : public ::google::protobuf::RpcChannel {
public:
    // 重写此方法
    void CallMethod(xxxx) override;
    // rpc请求的数组组装，数据的序列化
    // 发送rpc请求，wait
    // 接收rpc响应
    // 响应的反序列化
};


```

## 总结下如何发布一个服务

先写proto

protoc生成了pb之后

继承相应的类，然后重写方法，写业务函数

## controller

rpc服务的调用方

rpc 转到 channel的callMethod

如果中间出错直接return，网络没办法返回response

从名字上看出，可以存储一些控制信息，知道当前是什么状态

是个抽象类，里面都是纯虚函数

需要继承重写一下这些方法

