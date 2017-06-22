# Introduction #

`MongoDB` is an open-source document database that provides high performance, high availability, and automatic scaling.

> `MongoDB`是一个开源的文档式数据库，承诺提供高性能／高可用性／自动伸缩（能结合`Docker`然后自动生成销毁容器就更加完美）

## Document Database ##

A record in `MongoDB` is a `document`, which is a data structure composed of field and value pairs. `MongoDB` `documents` are similar to `JSON` objects. The values of fields may include other `documents`, arrays, and arrays of `documents`.

> `MongoDB`中的`document`就是记录
>
> 换言之，`document`就是表项，也和`JSON`对象很相视
>
> 键值对中的值可以是常见的类型（整数／字符串等），也可以是数组，还可以是另一个`document`

![1](1.svg)

The advantages of using `documents` are:

+ `Documents` (i.e. objects) correspond to native data types in many programming languages.

  > `Documents`与编程语言的对象具有非常明显的一一对应关系
  >
  > 省掉`ORM`

+ Embedded `documents` and arrays reduce need for expensive joins.

  > 支持`JSON`树使得表关联的需要被降低

+ Dynamic schema supports fluent polymorphism.

  > 动态规则支持流体多态
  >
  > 什么是动态规则？
  >
  > 什么是流体多态？

## Key features ##

### High Performance ###

`MongoDB` provides high performance data persistence. In particular,

> `MongoDB`提供高性能的数据持久化，特别是：

+ Support for embedded data models reduces I/O activity on database system.

  > 支持内嵌的数据模型以减少IO请求的次数

+ Indexes support faster queries and can include keys from embedded `documents` and arrays.

### Rich Query Language ###

`MongoDB` supports a rich query language to support read and write operations (CRUD) as well as:

+ Data Aggregation
+ Text Search and Geospatial Queries.

# Getting Started #

`MongoDB Atlas` is a cloud-hosted service for provisioning, running, monitoring, and maintaining `MongoDB` deployments. It is a fast, easy, and free way to get started with `MongoDB`. To install and run `MongoDB` locally, see Install `MongoDB`.

> `MongoDB Atlas`是一种云数据存储服务，是收费的，显然我是不会去用的
>
> 那么，动手搭建一个`MongoDB`服务吧

## Install MongoDB ##

```dockerfile
FROM ubuntu:16.04
MAINTAINER demons 837940593@qq.com

ADD sources.list /etc/apt/sources.list
RUN apt-key adv --keyserver hkp://keyserver.ubuntu.com:80 --recv 0C49F3730359A14518585931BC711F9BA15703C6
RUN echo "deb [ arch=amd64,arm64 ] http://repo.mongodb.org/apt/ubuntu xenial/mongodb-org/3.4 multiverse" \
    | tee /etc/apt/sources.list.d/mongodb-org-3.4.list
RUN apt-get update

RUN apt-get install -y mongodb-org

# ENTRYPOINT service mongod start
```

因为众所周知的原因，需要把`MongoDB`仓库换成阿里云提供的仓库（在此感谢马云爸爸）

```dockerfile
RUN echo "deb http://mirrors.aliyun.com/mongodb/apt/ubuntu xenial/mongodb-org/3.4 multiverse" \
    | tee /etc/apt/sources.list.d/mongodb-org-3.4.list
```

```text
deb http://mirrors.163.com/ubuntu/ wily main restricted universe multiverse
deb http://mirrors.163.com/ubuntu/ wily-security main restricted universe multiverse
deb http://mirrors.163.com/ubuntu/ wily-updates main restricted universe multiverse
deb http://mirrors.163.com/ubuntu/ wily-proposed main restricted universe multiverse
deb http://mirrors.163.com/ubuntu/ wily-backports main restricted universe multiverse
deb-src http://mirrors.163.com/ubuntu/ wily main restricted universe multiverse
deb-src http://mirrors.163.com/ubuntu/ wily-security main restricted universe multiverse
deb-src http://mirrors.163.com/ubuntu/ wily-updates main restricted universe multiverse
deb-src http://mirrors.163.com/ubuntu/ wily-proposed main restricted universe multiverse
deb-src http://mirrors.163.com/ubuntu/ wily-backports main restricted universe multiverse
```

```shell
docker build -t mongodb .
```

## Use Atlas ##

## Documents and Collections ##

`MongoDB` stores data as `BSON` `documents` (binary represenatation of JSON) in `collections`. `MongoDB` `databases` hold `collections` of `documents`.

> `MongoDB` 以`BSON`的形式存储数据
>
> `database` 包含多个`collection`，`collection`包含多个`document`

### Insert Documents ###

`db.collection.insertMany()` can insert multiple `documents` into a `collection`. Pass an array of `documents` to the method.

> `db.collection.insertMany()`可以插入多个`document`
>
> 该方法接受一个数组作为参数

The following example inserts new `documents` into the `inventory` `collection`:

```javascript
db.inventory.insertMany([
   // MongoDB adds the _id field with an ObjectId if _id is not present
   { item: "journal", qty: 25, status: "A",
       size: { h: 14, w: 21, uom: "cm" }, tags: [ "blank", "red" ] },
   { item: "notebook", qty: 50, status: "A",
       size: { h: 8.5, w: 11, uom: "in" }, tags: [ "red", "blank" ] },
   { item: "paper", qty: 100, status: "D",
       size: { h: 8.5, w: 11, uom: "in" }, tags: [ "red", "blank", "plain" ] },
   { item: "planner", qty: 75, status: "D",
       size: { h: 22.85, w: 30, uom: "cm" }, tags: [ "blank", "red" ] },
   { item: "postcard", qty: 45, status: "A",
       size: { h: 10, w: 15.25, uom: "cm" }, tags: [ "blue" ] }
]);
```

`insertMany()` returns a `document` that includes the newly inserted `documents` _id field values. 

Use `db.collection.insertOne()` to insert a single `document`.

> 如果只需要插入一个`document`，使用`db.collection.insertOne()`方法

### Query Documents ###

#### Select All Documents ####

To select all `documents` in the `collection`, pass an empty `document` as the query filter `document` to the `db.collection.find()` method:

> 通过给`db.collection.find()`方法传入一个空`document`作为参数来选择`collection`中的所有`document`

```javascript
db.inventory.find( {} )
```

To query for `documents` that match specific equality conditions, pass the `find()` method a query filter `document` with the `<field>: <value>` of the desired `documents`. The following example selects from the `inventory` `collection` all `documents` where the `status` equals `"D"`:

> 如果需要查找某一项属性等于某个值的`document`，可以相应的传入键值对集合

```javascript
db.inventory.find( { status: "D" } )
```

#### Match an Embedded Document ####

Equality matches on the whole embedded `document` require an *exact* match of the specified `<value>` `document`, including the field order. For example, the following query selects all `documents` where the field `size` equals the document `{ h: 14, w: 21, uom: "cm" }`:

> 如果需要匹配属性中的属性，也出人意料地合理／简单
>
> 只需要在查询集合中嵌套查询集合即可

```javascript
db.inventory.find( { size: { h: 14, w: 21, uom: "cm" } } )
```

#### Match a Field in an Embedded Document ####

The following example selects all `documents` where the field `uom` nested in the `size` field equals the string value `"in"`:

> 这可以认为是Match an Embedded Document在一种特殊情况下的语法糖

```javascript
db.inventory.find( { "size.uom": "in" } )
```

#### Match an Element in an Array ####

The following example queries for all `documents` where `tags` is an array that contains the string `"red"` as one of its elements:

```javascript
db.inventory.find( { tags: "red" } )
```

#### Match an Array Exactly ####

The following example queries for all `documents` where the field `tags` value is an array with exactly two elements, `"red"` and `"blank"`, in the specified order:

> 注意顺序都是需要一样的

```javascript
db.inventory.find( { tags: ["red", "blank"] } )
```

# Databases and Collections #

`MongoDB` stores `BSON` `documents`, i.e. data records, in `collections`; the `collections` in `databases`.

![2](2.svg)

## Databases ##

由于使用`Docker`，不具备localhost，所以直接启动`Mongo`会导致错误

![3](3.png)

```shell
vim /etc/mongod.conf
```

```text
# network interfaces
net:
  port: 27017
  bindIp: 0.0.0.0
```

```shell
mongod --config /etc/mongod.conf &
mongo
```

In `MongoDB`, `databases` hold `collections` of `documents`.

To select a `database` to use, in the mongo shell, issue the use `<db>` statement, as in the following example:

```shell
use myDB
```

### Create a Database ###

If a `database` does not exist, `MongoDB` creates the `database` when you first store data for that `database`. As such, you can switch to a non-existent `database` and perform the following operation in the mongo shell:

```shell
use myNewDB
db.myNewCollection1.insertOne( { x: 1 } )
```

> 如果数据库不存在，在插入第一个`collection`的时候就会自动创建
>
> 还可以看出，如果`collection`不存在，插入第一个`document`的时候也会自动创建

The `insertOne()` operation creates both the `database` `myNewDB` and the `collection` `myNewCollection1` if they do not already exist.

## Collections ##

`MongoDB` stores `documents` in `collections`. `Collections` are analogous to tables in relational databases.

> 不再说`database`／`collection`／`document`三个概念

### Create a Collection ###

If a `collection` does not exist, `MongoDB` creates the `collection` when you first store data for that `collection`.

```shell
db.myNewCollection2.insertOne( { x: 1 } )
db.myNewCollection3.createIndex( { y: 1 } )
```

Both the `insertOne()` and the `createIndex()` operations create their respective `collection` if they do not already exist.

![4](4.png)

### Explicit Creation ###

`MongoDB` provides the `db.createCollection()` method to explicitly create a `collection` with various options, such as setting the maximum size or the documentation validation rules. If you are not specifying these options, you do not need to explicitly create the collection since MongoDB creates new `collections` when you first store data for the collections.

> `MongoDB`提供`db.createCollection()`方法去显式创建一个`collection`
>
> 这个方法接受很多描述这个`collection`性质的参数，如果不需要使用这个参数，就不需要使用这个方法

### Document Validation ###

By default, a `collection` does not require its `documents` to have the same schema; i.e. the `documents` in a single `collection` do not need to have the same set of fields and the data type for a field can differ across `documents` within a `collection`.

> 即为数据添加验证规则

### Modifying Document Structure ###

To change the structure of the `documents` in a `collection`, such as add new fields, remove existing fields, or change the field values to a new type, update the `documents` to the new structure.

## Views ##

Starting in version 3.4, `MongoDB` adds support for creating read-only `views` from existing `collections` or other `views`.

> 这是从传统关系型数据库中借鉴的概念吗？
>
> 是数据的一种表现形式？

### Create View ###

To create or define a `view`, `MongoDB` 3.4 introduces:

+ the `viewOn` and `pipeline` options to the existing `create` command (and `db.createCollection` helper)

  ```shell
  db.runCommand( { create: <view>, viewOn: <source>, pipeline: <pipeline> } )
  ```

  or if specifying a default `collation` for the view:

  ```shell
  db.runCommand( { create: <view>, viewOn: <source>, pipeline: <pipeline>, collation: <collation> } )
  ```


+ a new mongo shell helper `db.createView()`

  ```shell
  db.createView(<view>, <source>, <pipeline>, <collation> )
  ```

### Behavior ###

+ Read only（只读）

+ Index Use and Sort Operations

  + Views use indexes of the underlying collection.

    > 类似于迭代器的概念？或者是MPL中的视图概念？
    >
    > 反正是一种效率优于拷贝的实现？

  + You cannot specify a `$natural` sort on a view.

+ Projection Restrictions

+ Immutable Name

+ View Creation

+ Sharded View

+ Views and Collation

+ Public View Definition

### Drop a view ###

To remove a view, use the `db.collection.drop()` method on the view.

## Capped Collections ##

# Documents #

## Document Structure ##

## Dot Notation ##

## Document Limitations ##

## Other Uses of the Document Structure ##

## Additional Resources ##

# BSON Type #

# Comparison/Sort Order #

# MongoDB Extended JSON #

