# Aggregation #

Aggregation operations process data records and return computed results. Aggregation operations group values from multiple `documents` together, and can perform a variety of operations on the grouped data to return a single result. `MongoDB` provides three ways to perform aggregation: the aggregation pipeline, the map-reduce function, and single purpose aggregation methods.

> Aggregation就是对数据库内的数据进行处理
>
> `MongoDB`提供了两种处理模型（它们都是高度可定制化的）和一种满足简单需求的处理方式

## Aggregation Pipeline ##

`MongoDB`’s aggregation framework is modeled on the concept of **data processing pipelines**. `Documents` enter a multi-stage pipeline that transforms the ``documents` into an aggregated result.

> `MongoDB`的数据处理框架是立足于数据处理流水线的，是一个多阶段的过程

The most basic pipeline stages provide **filters** that operate like queries and **document transformations** that modify the form of the output `document`.

> 最基本的阶段：如何吧数据选出来，如何把数据转换成更好处理的形式

Other pipeline operations provide tools for **grouping** and **sorting** `documents` by specific field or fields as well as tools for **aggregating the contents of arrays**, including arrays of `documents`. In addition, pipeline stages can use operators for tasks such as **calculating the average** or **concatenating a string**.

> 第二个阶段：如何吧数据分组，对分组过的数据做什么处理
>
> 如果在这个阶段不支持用户提交自定义的函数的话，这种模型的自由度还是欠缺的，好处可能是效率会更高（限制用户操作的可能性，对剩下的可能性进行特定的优化）

The pipeline provides efficient data aggregation using native operations within `MongoDB`, and is the preferred method for data aggregation in `MongoDB`.

> 看来我的猜测是对的，这种数据处理模型不支持用户自定义操作
>
> 而自带的操作都很贴近于`MongoDB`的原生操作，所以效率会很高

The aggregation pipeline can operate on a **sharded** `collection`.

> 共享`collection`？意思是可以从多张表中选取数据？

The aggregation pipeline can use indexes to improve its performance during some of its stages.

> 在某些阶段可以用索引提升处理性能，这种高级话题我现在还是不参与了

In addition, the aggregation pipeline has an internal optimization phase.

> 相信自带的性能优化器该有多棒棒🍭

![1](1.jpg)

## Map-Reduce ##

`MongoDB` also provides map-reduce operations to perform aggregation.

> Hadoop采用的数据处理模型，现在几乎成为一种数据处理框架的标准
>
> 话说，`MongoDB`的数据库集群架构会不会和Hadoop的架构是一样的？

In general, map-reduce operations have two phases: a map stage that processes each `document` and emits one or more objects for each input `document`, and reduce phase that combines the output of the map operation.

> 分为两个阶段：map（把`document`转换成对象），reduce（归约对象）

Optionally, map-reduce can have a finalize stage to make final modifications to the result.

> 可以额外地拥有一个阶段（finalize）去对结果做最终的修改

Like other aggregation operations, map-reduce can specify a query condition to select the input `documents` as well as sort and limit the results.

Map-reduce uses custom JavaScript functions to perform the map and reduce operations, as well as the optional finalize operation. While the custom JavaScript provide great flexibility compared to the aggregation pipeline, in general, map-reduce is less efficient and more complex than the aggregation pipeline.

> 用性能换自由度
>
> 建议如果能用Aggregation Pipeline解决的都用Aggregation Pipeline解决，部分轻型请求可以用Map-Reduce，其他计算量很大的操作请把数据取出来在客户端操作

Map-reduce can operate on a **sharded** `collection`. Map reduce operations can also output to a **sharded** `collection`.

![2](2.jpg)



## Single Purpose Aggregation Operations ##

`MongoDB` also provides **db.collection.count()** and **db.collection.distinct()**.

All of these operations aggregate `documents` from a **single** `collection`.

While these operations provide simple access to common aggregation processes, they lack the flexibility and capabilities of the aggregation pipeline and map-reduce.

![3](3.jpg)

## Additional Features and Behaviors ##

## Additional Resources ##

# Aggregation Pipeline #

# Map-Reduce #

Map-reduce is a data processing paradigm for condensing large volumes of data into useful aggregated results. For map-reduce operations, `MongoDB` provides the **mapReduce** database command.

> Map-reduce是一种数据处理的通用范式

Consider the following map-reduce operation:

![4](4.jpg)

In this map-reduce operation, `MongoDB` applies the map phase to each input `document` (i.e. the `documents` in the `collection` that match the query condition). The map function emits key-value pairs. For those keys that have multiple values, `MongoDB` applies the reduce phase, which collects and condenses the aggregated data. `MongoDB` then stores the results in a `collection`. Optionally, the output of the reduce function may pass through a finalize function to further condense or process the results of the aggregation.

> 第一个阶段负责发射名值对（一个名对应于一个值），如果综合之后，一个名对应多个值，则进入下一个阶段
>
> map-reduce之后可以有一个可选的阶段，决定怎么利用得到的结果；默认的处理方式是把这些结果存储到另一个表

All map-reduce functions in `MongoDB` are JavaScript and run within the mongod process. Map-reduce operations take the `documents` of a single `collection` as the input and can perform any arbitrary sorting and limiting before beginning the map stage. mapReduce can return the results of a map-reduce operation as a `document`, or may write the results to `collections`. The input and the output `collections` may be sharded.

> map-reduce发生在服务器
>
> 这不是废话吗，如果发生在客户端我干嘛不直接把数据取回来自己慢慢改？？

## Map-Reduce JavaScript Functions ##

In `MongoDB`, map-reduce operations use custom JavaScript functions to map, or associate, values to a key. If a key has multiple values mapped to it, the operation reduces the values for the key to a single object.

The use of custom JavaScript functions provide flexibility to map-reduce operations. For instance, when processing a `document`, the map function can create more than one key and value mapping or no mapping. Map-reduce operations can also use a custom JavaScript function to make final modifications to the results at the end of the map and reduce operation, such as perform additional calculations.

> 很多是重复之前的东西（好气啊！！！废话怎么这么多！！你们的文档时复制粘贴写出来的吗！！）
>
> 强调一下自由度

## Map-Reduce Behavior ##

In `MongoDB`, the map-reduce operation can write results to a `collection` or return the results inline. If you write map-reduce output to a `collection`, you can perform subsequent map-reduce operations on the same input `collection` that merge replace, merge, or reduce new results with previous results. See mapReduce and Perform Incremental Map-Reduce for details and examples.

> map-reduce的结果有多种用途，可以是重新写回数据库，也可以是直接返回
>
> 如果直接写回数据库，还可以再次进行map-reduce

When returning the results of a map reduce operation inline, the result `documents` must be within the BSON Document Size limit, which is currently 16 megabytes. For additional information on limits and restrictions on map-reduce operations, see the mapReduce reference page.

> 仍然有大小限制

`MongoDB` supports map-reduce operations on sharded `collections`. Map-reduce operations can also output the results to a sharded `collection`. See Map-Reduce and Sharded Collections.

> map-reduce可以用于shard `collections`

Views do not support map-reduce operations.

> 视图不支持map-reduce操作
