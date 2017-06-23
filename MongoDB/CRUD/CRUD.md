# MongoDB CRUD Operations #

CRUD operations create, read, update, and delete `documents`.

> CRUD就是传统数据库的基本操作

## Create Operations ##

Create or insert operations add new `documents` to a `collection`.

> 创建／插入操作会在`collection`中添加一个新的`document`

If the `collection` does not currently exist, insert operations will create the `collection`.

> 如果连`collection`都不存在，插入操作会创建这个`collection`

`MongoDB` provides the following methods to insert `documents` into a `collection`:

> `MongoDB`提供以下几个方法去插入`document`

+ db.collection.insertOne()
+ db.collection.insertMany()

In `MongoDB`, insert operations target a single `collection`.

> 插入操作的目标是单个`collection`，也就是说同时插入多个`collection`是不可行的

All write operations in MongoDB are `atomic` on the level of a single `document`.

> 在`document`层次的写入操作是原子性的
>
> `MongoDB`能不能保证几个写入操作是原子性的？比如写入多个`document`或者向多个`collection`写入？
>
> 这种组合原子操作使之成为原子操作的能力十分重要

![1](1.jpg)

![2](2.jpg)

![3](3.jpg)

## Read Operations ##

Read operations retrieves `documents` from a `collection`; i.e. queries a `collection` for `documents`.

> 读操作的目的是从`collection`中取出`document`，对于数据库来说，读操作就是查询操作

`MongoDB` provides the following methods to read documents from a `collection`:

> 查询操作在前面已经有意无意地提到过很多次

+ db.collection.find()

![4](4.jpg)

> 我还是想说，如果不支持用户传入闭包指定查询条件，`MongoDB`的查询方法的适用性有限

## Update Operations ##

Update operations modify existing `documents` in a `collection`. MongoDB provides the following methods to update `documents` of a `collection`:

> 修改操作一定要支持部分属性的升级

+ db.collection.updateOne()
+ db.collection.updateMany()
+ db.collection.replaceOne()

In `MongoDB`, update operations target a single `collection`. All write operations in `MongoDB` are atomic on the level of a single `document`.

> 同样，升级操作也保证`document`层次的原子性，也就是说，不可能一个属性升级成功了，而另一个属性升级失败了

You can specify criteria, or filters, that identify the documents to update. These filters use the same syntax as read operations.

> 升级操作甚至允许用条件选定需要升级的`document`

![5](5.jpg)

如果filter条件未空，则选择第一个`document`

![6](6.jpg)

事实上，如果条件不为空但是却选中多个`document`，仍然是升级第一个`document`

**updateOne**值得详细研究，尤其是其参数

猜想，**updateMany()**方法就是升级所有选中的`document`

![7](7.jpg)

自然，**replaceMany**方法就是替换掉整个`document`，而不是对属性值替换

## Delete Operations ##

Delete operations remove `documents` from a `collection`.

`MongoDB` provides the following methods to delete `documents` of a `collection`:

+ db.collection.deleteOne()
+ db.collection.deleteMany()

In `MongoDB`, delete operations target a single `collection`. All write operations in `MongoDB` are atomic on the level of a single `document`.

> 同样提供原子性保证

You can specify criteria, or filters, that identify the `documents` to remove. These filters use the same syntax as read operations.

![8](8.jpg)

## Bulk Write ##

`MongoDB` provides the ability to perform write operations in bulk. For details, see Bulk Write Operations.

> 大概就是把写操作组合在一起，减少通信的次数
>
> 有没有对这些绑在一起的写操作提供原子性呢？（始终关心）

# Insert Documents #

If the collection does not currently exist, insert operations will create the collection.

> 哎呀，又来……

## Insert a Single Document ##

**db.collection.insertOne()** inserts a single `document` into a `collection`.

> 单个`document`的插入

The following example inserts a new `document` into the **inventory** `collection`.

If the document does not specify an **_id** field, MongoDB adds the **_id** field with an **ObjectId** value to the new `document`.

> 如果在插入的时候确实**_id**域，则自动生成
>
> `_id`作为主键，十分重要
>
> 一般的应用不要去干涉`_id`的生成过程

```javascript
db.inventory.insertOne(
   { item: "canvas", qty: 100, tags: ["cotton"], size: { h: 28, w: 35.5, uom: "cm" } }
)
```

**insertOne()** returns a `document` that includes the newly inserted `document`’s **_id** field value.

## Insert Multiple Documents ##

**db.collection.insertMany()** can insert multiple `documents` into a `collection`. Pass an array of `documents` to the method.

```javascript
db.inventory.insertMany([
   { item: "journal", qty: 25, tags: ["blank", "red"], size: { h: 14, w: 21, uom: "cm" } },
   { item: "mat", qty: 85, tags: ["gray"], size: { h: 27.9, w: 35.5, uom: "cm" } },
   { item: "mousepad", qty: 25, tags: ["gel", "blue"], size: { h: 19, w: 22.85, uom: "cm" } }
])
```

## Insert Behavior ##

### Collection Creation ###

If the `collection` does not currently exist, insert operations will create the `collection`.

> 额……，😂

### _id Field ###

In `MongoDB`, each `document` stored in a `collection` requires a unique **_id** field that acts as a primary key. If an inserted `document` omits the **_id** field, the `MongoDB` driver automatically generates an **ObjectId** for the **_id** field.

> 主键，自动生成

This also applies to `documents` inserted through update operations with upsert: true.

### Atomicity ###

All write operations in `MongoDB` are atomic on the level of a single `document`.

> `document`层次的原子性保证
>
> 我关心的是原子操作的组合

### Write Acknowledgement ###

With write concerns, you can specify the level of acknowledgement requested from `MongoDB` for write operations.

> 感情这就是个预告？？(｡ì _ í｡)

## Insert Methods ##

# Query Documents #

## Query on Embedded/Nested Documents ##

## Query an Array ##

## Query an Array of Embedded Documents ##

## Project Fields to Return from Query ##

## Query for Null or Missing Fields ##

## Iterate a Cursor in the mongo Shell ##

# Update Documents #

## Update Methods ##

# Delete Documents #

## Delete Methods ##

# Bulk Write Operations #

# SQL to MongoDB Mapping Chart #

# Read Concern #

# Write Concern #

# MongoDB CRUD Concepts #

## Atomicity and Transactions ##

## Read Isolation, Consistency, and Recency ##

## Distributed Queries ##

## Distributed Write Operations ##

## Perform Two Phase Commits ##

## Linearizable Reads via findAndModify ##

## Query Plans ##

## Query Optimization ##

### Evaluate Performance of Current Operations ###

### Optimize Query Performance ###

### Write Operation Performance ###

### Explain Results ###

## Analyze Query Performance ##

## Tailable Cursors ##
