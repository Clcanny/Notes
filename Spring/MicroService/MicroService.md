# Microservices with Spring #

A simple example of setting up a microservices system using Spring, Spring Boot and Spring Cloud.

Microservices allow large systems to be built up from a number of collaborating components.

> 微服务的核心思想是将一个大的软件系统拆分成相对独立的几个组件（也叫服务），服务具有相对独立性
>
> 微服务的好处：运行时可根据需求动态调节服务的多少，调节的单位从以前的整个软件变成了某个组件；开发时各小组具有独立性；组件进一步解耦，使得组件的替换变得容易很多

It does at the process level what Spring has always done at the component level: loosely coupled processes instead of loosely coupled components.

> 解耦进程替代解耦组件

For example imagine an online shop with separate microservices for user-accounts, product-catalog order-processing and shopping carts:

> 想象一下一个电商服务可以分解成几个微服务：用户账户的管理／产品分类／订单管理／购物车

![1](1.jpg)

Inevitably there are a number of moving parts that you have to setup and configure to build such a system. How to get them working together is not obvious - you need to have good familiarity with Spring Boot since Spring Cloud leverages it heavily, several Netflix or other OSS projects are required and, of course, there is some Spring configuration “magic”!

> 微服务的难点是怎么把几个微服务组合在一起
>
> 这需要我们对Spring本身的理解，还需要借助Netflix开发的软件（用于微服务之间互相发现）等

![2](2.jpg)

上面的架构图提醒我们三点：

1. 进程／线程间通信变成网络通信
2. 请求分发功能原本是依赖于Spring框架去做的，是隐式的／无须程序员干预的，变成显式的／需要程序员干预的
3. Eureka提供微服务之间互相发现的功能，类似于P2P／Hadoop中的名服务

In this article I aim to clarify how things work by building the simplest possible system step-by-step. Therefore, I will only implement a small part of the big system - the user account service.

> 很好，一步一步来，不过只实现其中一个微服务
>
> 不过也没有关系，重要的是这些微服务如何一起工作，其它的使用旧知识就足够了
>
> 而且我马上有两个项目就会用到微服务

The Web-Application will make requests to the Account-Service microservice using a RESTful API. We will also need to add a discovery service – so the other processes can find each other.

> 而且，无状态豆RESTful对微服务欧莫名友好，根本不需要考虑session之类的共享
>
> 不过，所有的水平向扩展都需要解决数据库如何扩展的问题，尤其是关系型数据库，水平向扩展让人头疼

## Follow-Up 1: Other Resources ##

This article only discusses a minimal system. For more information, you might like to read Josh Long’s blog article **Microservice Registration and Discovery with Spring Cloud and Netflix’s Eureka** which shows running a complete microservice system on Cloud Foundry.

## Follow Up 2: SpringOne Platform 2016 ##

Book your place at SpringOne2 Platform in Las Vegas, USA soon - simply the best opportunity to find out first hand all that’s going on and to provide direct feedback. The name has changed, from Spring One, to reflect the growth of Spring in platform services (such as the Spring Cloud projects)

> 这个广告真是……猝不及防……

## Service Registration ##

When you have multiple processes working together they need to find each other. If you have ever used Java’s RMI mechanism you may recall that it relied on a central registry so that RMI processes could find each other. Microservices has the same requirement.

> 不管是进程还是微服务，都需要通过一种途径进行互相发现

The developers at Netflix had this problem when building their systems and created a registration server called Eureka (“I have found it” in Greek). Fortunately for us, they made their discovery server open-source and Spring has incorporated into Spring Cloud, making it even easier to run up a Eureka server.

> Eureka这个名字好，“我发现”听上去就很牛逼的样子

Here is the complete discovery-server application:

```java
@SpringBootApplication
@EnableEurekaServer
public class ServiceRegistrationServer {

  public static void main(String[] args) {
    // Tell Boot to look for registration-server.yml
    System.setProperty("spring.config.name", "registration-server");
    SpringApplication.run(ServiceRegistrationServer.class, args);
  }
}
```

如果需要配置，说明Eureka或者Spring Boot的工作还没有做到家

理论上来说，Eureka只需要接受到某个请求说：我是某某微服务和一个验证token，Eureka就应该进行登记工作（IP地址就是请求的IP，名字就是“某某微服务”）

It really is that simple!

> 虽然这个配置项可能很简单，但是再简单也是一个配置项

Spring Cloud is built on Spring Boot and utilizes parent and starter POMs. The important parts of the POM are:

> 需要安装一些依赖项：

```xml
<parent>
    <groupId>org.springframework.cloud</groupId>
    <artifactId>spring-cloud-starter-parent</artifactId>
    <version>_Brixton_.RELEASE</version>  <!-- Name of release train -->
</parent>
<dependencies>
    <dependency>
        <!-- Setup Spring Boot -->
        <groupId>org.springframework.boot</groupId>
        <artifactId>spring-boot-starter</artifactId>
    </dependency>

    <dependency>
        <!-- Setup Spring MVC & REST, use Embedded Tomcat -->
        <groupId>org.springframework.boot</groupId>
        <artifactId>spring-boot-starter-web</artifactId>
    </dependency>

    <dependency>
        <!-- Spring Cloud starter -->
        <groupId>org.springframework.cloud</groupId>
        <artifactId>spring-cloud-starter</artifactId>
    </dependency>

    <dependency>
        <!-- Eureka for service registration -->
        <groupId>org.springframework.cloud</groupId>
        <artifactId>spring-cloud-starter-eureka-server</artifactId>
    </dependency>
</dependencies>
```

By default Spring Boot applications look for an `application.properties` or `application.yml` file for configuration. By setting the **spring.config.name property** we can tell Spring Boot to look for a different file - useful if you have **multiple** Spring Boot applications in the same project - as I will do shortly.

> 这种操作很厉害，原来不会丢失原本的配置文件啊

This application looks for `registration-server.properties` or `registration-server.yml`. Here is the relevant configuration from `registration-server.yml`:

```properties
# Configure this Discovery Server
eureka:
  instance:
    hostname: localhost
  client:  # Not a client, don't register with yourself
    registerWithEureka: false
    fetchRegistry: false
in
server:
  port: 1111   # HTTP (Tomcat) port
```

By default Eureka runs on port 8761, but here we will use port 1111 instead. Also by including the registration code in my process I might be a server or a client. The configuration specifies that I am not a client and stops the server process trying to register with itself.

> 错怪Eureka，这些配置是很简单且必须的

Spring Cloud also supports Consul as an alternative to Eureka. You start the Consul Agent (its registration server) using a script and then clients use it to find their microservices.

> 不过还可以用Consul替代Eureka

## Creating a Microservice: Account-Service ##

A microservice is a stand-alone process that handles a well-defined requirement.

When configuring applications with Spring we emphasize Loose Coupling and Tight Cohesion, These are not new concepts (Larry Constantine is credited with first defining these in the late 1960s - reference) but now we are applying them, not to interacting components (Spring Beans), but to interacting processes.

> 高内聚松耦合都喊了多少年了……，说明很重要，嗯……
>
> 原先解耦的方式是组件（体现就是beans），现在解耦的方式是微服务（也就是程序／进程）

![3](3.jpg)

In this example, I have a simple Account management microservice that uses Spring Data to implement a JPA AccountRepository and Spring REST to provide a RESTful interface to account information. In most respects this is a straightforward Spring Boot application.

> 剩下的都是常用做法，利用Spring Data去帮我们自动实现JPA，保持REST的接口

What makes it special is that it registers itself with the discovery-server at start-up. Here is the Spring Boot startup class:

```java
@EnableAutoConfiguration
@EnableDiscoveryClient
@Import(AccountsWebApplication.class)
public class AccountsServer {

    @Autowired
    AccountRepository accountRepository;

    public static void main(String[] args) {
        // Will configure using accounts-server.yml
        System.setProperty("spring.config.name", "accounts-server");

        SpringApplication.run(AccountsServer.class, args);
    }
}
```

The annotations do the work:

1. @EnableAutoConfiguration - defines this as a Spring Boot application.

2. @EnableDiscoveryClient - this enables service registration and discovery. In this case, this process registers itself with the discovery-server service using its application name (see below).

   > 这个annotations会让AccountsServer在启动的时候向Eureka注册一下

3. @Import(AccountsWebApplication.class) - this Java Configuration class sets up everything else (see below for more details).

What makes this a microservice is the registration with the discovery-server via @EnableDiscoveryClient and its YML configuration completes the setup:

```yaml
# Spring properties
spring:
  application:
     name: accounts-service

# Discovery Server Access
eureka:
  client:
    serviceUrl:
      defaultZone: http://localhost:1111/eureka/

# HTTP Server
server:
  port: 2222   # HTTP (Tomcat) port
```

Note that this file:

+ Sets the application name as accounts-service. This service registers under this name and can also be accessed by this name - see below.
+ Specifies a custom port to listen on (2222). All my processes are using Tomcat, they can’t all listen on port 8080.
+ The URL of the Eureka Service process - from the previous section.


使用浏览器[打开](http://localhost:1111)可以查看注册情况

使用postman[打开](http://localhost:1111/eureka/apps/)可以查看更加详细的信息

## Configuration Options ##

### Registration Time ###

Registration takes up to 30s because that is the default client refresh time. You can change this by setting the **eureka.instance.leaseRenewalIntervalInSeconds** property to a smaller number (in the demo application I have set it to 5). This is not recommended in production. 

> 既然在生产环境中不推荐，那么这一项就没有什么意义，也不是微服务的重点

```properties
eureka:
  instance:
    leaseRenewalIntervalInSeconds: 5         # DO NOT DO THIS IN PRODUCTION
```

### Registration Id ###

A process (microservice) registers with the discovery-service using a unique id. If another process registers with the same id, it is treated as a restart (for example some sort of failover or recovery) and the first process registration is discarded. This gives us the fault-tolerant system we desire.

> Eureka认为同一个id的多个微服务注册是错误恢复导致的
>
> 所以，如果需要运行一个微服务的多个实例，必须保证多个实例的id是不同的
>
> 比较简单的是让IP地址和端口号参与id的生成
>
> 如果使用docker进行部署，问题就变得复杂起来，因为对于微服务而言，它们的地址都是0.0.0.0，端口号都是8080

To run multiple instances of the same process (for load-balancing and resilience) they need to register with a unique id. When I first wrote this blog, that was automatic and since the Brixton release-train, it is again.

Under the Angel release train, the instance-id, used by a client to register with a discovery server, was derived from the client’s service name (the same as the Spring application name) and also the client’s host name. The same processes running on the same host would therefore have the same id, so only one could ever register.

> 难道作者写到这里的时候就没有考虑介绍一下**Brixton**和**Angel**吗？
>
> 正常的读者看到这里难道不是懵逼的？
>
> 经过百度，这两货是Spring Cloud的版本号，**Angle**是第一个版本，**Brixton**是第二个版本

Fortunately you could set the id property manually via the client’s Eureka metadata map, like this:

> 好像是第一个版本比较SB，所以需要额外的配置工作：

```properties
eureka:
  instance:
    metadataMap:
      instanceId: ${spring.application.name}:${spring.application.instance_id:${server.port}}
```

Since the Brixton release train, this is now the default. So what does it do?

> 从第二个版本开始，这个配置就是默认配置

让我猜一猜，这个默认配置的id都包括什么？

+ 微服务名称
+ 进程号
+ IP地址和端口号

We are setting the `instanceId` to `application-name:instance_id`, but if `instance_id` is not defined, we will use `application-name::server-port` instead.

我上面的猜测稍微有些错误

如果`instanceId`存在则使用`instanceId`，否则用IP地址和端口号替代

Note that the `spring.application.instance_id` is only set when using **Cloud Foundry** but it conveniently provides a unique id number for each instance of the same application.

> 看上去这个`instanceId`还蛮可靠的样子

We can do something similar when running elsewhere by using the server-port (since different instances on the same machine must listen on different ports.

> 如果不同docker，*IP地址+端口号唯一确定微服务实例*是对的，但是如果使用docker，呵呵
>
> 我个人建议添加一个时间作为种子的随机数

Another example you will often see is `\${spring.application.name}:\${spring.application.instance_id:\${random.value}}`

这个挺不错的，就是随机数的质量往往决定节点的使用效率（如果两个随机数一样，则导致其中一个微服务实例不能注册，因而一点用也没有）

but I personally find using the port number makes each instance easy to identify - the random values are just long strings that don’t mean anything.

> 使用随机数的坏处之一就是根本没有任何物理意义

Note: The syntax `\${x:\${y}}` is Spring property shorthand for `\${x} != null ? \${x} : \${y}`.

> 语法普及

Since the Brixton release there is also a dedicated property for this:

```properties
eureka:
  instance:
    instanceId: ${spring.application.name}:${spring.application.instance_id:${random.value}}
```

## Accessing the Microservice: Web-Service ##

To consume a RESTful service, Spring provides the **RestTemplate** class. This allows you to send HTTP requests to a RESTful server and fetch data in a number of formats - such as JSON and XML.

> **RestTemplate**允许用户发送HTTP请求，取回各种形式的数据
>
> 关键是我不用**RestTemplate**也能很方便地向服务器发送HTTP请求，然后取回JSON格式数据啊？

A microservice (discovery) client can use a **RestTemplate** and Spring will automatically configure it to be microservice aware (more of this in a moment).

### Encapsulating Microservice Access ###

```java
@Service
public class WebAccountsService {

    @Autowired        // NO LONGER auto-created by Spring Cloud (see below)
    @LoadBalanced     // Explicitly request the load-balanced template
                      // with Ribbon built-in
    protected RestTemplate restTemplate; 

    protected String serviceUrl;

    public WebAccountsService(String serviceUrl) {
        this.serviceUrl = serviceUrl.startsWith("http") ?
               serviceUrl : "http://" + serviceUrl;
    }

    public Account getByNumber(String accountNumber) {
        Account account = restTemplate.getForObject(serviceUrl
                + "/accounts/{number}", Account.class, accountNumber);

        if (account == null)
            throw new AccountNotFoundException(accountNumber);
        else
            return account;
    }
    ...
}
```

> 如果只看这段代码，我的推测如下：
>
> RestTemplate的作用不是返回Response，而是解析Response

Note that my `WebAccountService` is just a wrapper for the `RestTemplate` **fetching** data from the microservice. The interesting parts are the `serviceUrl` and the `RestTemplate`.

> RestTemplate应该是一个Response的解析器，可以自动把各种格式的Response解析成对象
>
> 这才是使用微服务需要解决的问题：
>
> 即原本使用组件（Beans）双方发送的消息是一个个对象，非常方便；现在使用微服务，双方发送的很可能是JSON，这样JSON到对象的映射如果需要程序员手动维护也太蠢了一点
>
> 所以我们有了**RestTemplate**

> 我们可以大胆地猜测：`serviceUrl`就是另一个微服务的名字
>
> 首先，如果是另一个微服务实例的IP地址，这也太蠢了一点，并且Eureka一点用处都没有
>
> 其次，使用Eureka意味着所有的微服务的互相发现和调用都应该需要Eureka参与调度，因而让Eurkea解析URL应该是默认行为，故不需要额外的参数或者标记

### Accessing the Microservice ###

As shown below, the `serviceUrl` is provided by the main program to the `WebAccountController` (which in turn passes it to the `WebAccountService`):

```java
@SpringBootApplication
@EnableDiscoveryClient
@ComponentScan(useDefaultFilters=false)  // Disable component scanner
public class WebServer {

    // Case insensitive: could also use: http://accounts-service
    public static final String ACCOUNTS_SERVICE_URL
                                        = "http://ACCOUNTS-SERVICE";

    public static void main(String[] args) {
        // Will configure using web-server.yml
        System.setProperty("spring.config.name", "web-server");
        SpringApplication.run(WebServer.class, args);
    }

    @LoadBalanced    // Make sure to create the load-balanced template
    @Bean
    RestTemplate restTemplate() {
        return new RestTemplate();
    }

    /**
     * Account service calls microservice internally using provided URL.
     */
    @Bean
    public WebAccountsService accountsService() {
        return new WebAccountsService(ACCOUNTS_SERVICE_URL);
    }

    @Bean
    public WebAccountsController accountsController() {
         return new WebAccountsController
                       (accountsService());  // plug in account-service
    }
}
```

A few points to note:

1. The `WebController` is a typical Spring MVC view-based controller returning HTML. The application uses Thymeleaf as the view-technology (for generating dynamic HTML)

2. `WebServer` is also a `@EnableDiscoveryClient` but in this case as well as registering itself with the discovery-server (which is not necessary since it offers no services of its own) it uses Eureka to locate the account service.

   > 把`WebServer`注册成服务器是没有必要的一件事

3. The default component-scanner setup inherited from Spring Boot looks for `@Component` classes and, in this case, finds my `WebAccountController` and tries to create it. However, I want to create it myself, so I disable the scanner like this `@ComponentScan(useDefaultFilters=false)`.

   > 可以看到在上面代码的最后一段，作者自己创建`WebAccountsController`类型的`bean`
   >
   > 所以把组件的自动扫描关掉
   >
   > 这是作者选用的一种实现技巧，和微服务没有什么关系

4. The service-url I am passing to the `WebAccountController` is the name the service used to register itself with the discovery-server - by default this is the same as the `spring.application.name` for the process which is `account-service` - see `account-service.yml` above. The use of upper-case is not required but it does help emphasize that ACCOUNTS-SERVICE is a logical host (that will be obtained via discovery) not an actual host.

### Load Balanced RestTemplate ###

The `RestTemplate` bean will be **intercepted** and **auto-configured** by Spring Cloud (due to the `@LoadBalanced` annotation) to use a custom HttpRequestClient that uses Netflix **Ribbon** to do the microservice lookup. Ribbon is also a load-balancer so if you have multiple instances of a service available, it picks one for you. (Neither Eureka nor Consul on their own perform load-balancing so we use Ribbon to do it instead).

> 我更倾向于用Nigix做负载均衡

> 还有，恕我直言，这篇文档写得很烂

Note: From the Brixton Release Train (Spring Cloud 1.1.0.RELEASE), the RestTemplate is no longer created automatically. Previously it was created for you, which caused confusion and potential conflicts (sometimes Spring can be too helpful!).

Note that this instance is qualified using @LoadBalanced. (The annotation is itself annotated with @Qualifier - see here for details). Thus if you have more than one RestTemplate bean, you can make sure to inject the right one, like this:

```java
@Autowired
@LoadBalanced     // Make sure to inject the load-balanced template
protected RestTemplate restTemplate;
```

> 多个bean在类型上均匹配的情况如何处理就不在这里讨论

### Configuration ###

Below the relevant configuration from web-server.yml. It is used to:

```properties
# Spring Properties
spring:
  application:
     name: web-service

# Discovery Server Access
eureka:
  client:
    serviceUrl:
      defaultZone: http://localhost:1111/eureka/

# HTTP Server
server:
  port: 3333   # HTTP (Tomcat) port
```

## How to Run the Demo ##

mvn clean package



我还是要吐槽一下这篇guide，写得太垃圾好吗！！

让人都忍不住各种跳过！！！