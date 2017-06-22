# What is mongoDB? #

`MongoDB` is a `document` `database` with the scalability and flexibility that you want with the querying and indexing that you need.

> `MongoDB`是一个文档式数据库，它在查询和索引方面具备伸缩性和自由性

+ MongoDB **stores data in flexible, JSON-like documents**, meaning fields can vary from document to document and data structure can be changed over time.

  > `MongoDB`用一种JSON式的文档自由地存储数据
  >
  > 这意味着域（fields）和数据结构可以非常容易地变化

+ The document model **maps to the objects in your application code**, making data easy to work with.

  > 数据模型与应用需要使用到的对象一一对应（天然ORM），使得数据非常容易被使用

+ **Ad hoc queries, indexing, and real time aggregation** provide powerful ways to access and analyze your data.

  > 多态的查询和索引方式／实时聚合，为访问和分析数据提供了有力的支持

+ MongoDB is a **distributed database at its core**, so high availability, horizontal scaling, and geographic distribution are built in and easy to use.

  > `MongoDB`是一种分布式数据库，这意味着它可以容易地水平向扩展／高可用性／按地理分布

+ MongoDB is **free and open-source**, published under the GNU Affero General Public License.

  > 成本低廉，代码开源



# Makes development easy #

## Connect ##

```java
// 1. Connect to MongoDB instance running on localhost
MongoClient mongoClient = new MongoClient();

// Access database named 'test'
MongoDatabase database = mongoClient.getDatabase("test");

// Access collection named 'restaurants'
MongoCollection<Document> collection = database.getCollection("restaurants");
```

Here we are connecting to a locally hosted `MongoDB` `database` called testwith a `collection` named restaurants.

`MongoCollection`是什么概念？是否对应着表？还是对应着表项？

## Insert a document ##

```java
// 2. Insert 
List<Document> documents = asList(
  new Document("name", "Sun Bakery Trattoria")
    .append("stars", 4)
    .append("categories",
      asList("Pizza", "Pasta", "Italian", "Coffee", "Sandwiches")),
  new Document("name", "Blue Bagels Grill")
    .append("stars", 3)
    .append("categories",
      asList("Bagels", "Cookies", "Sandwiches")),
  new Document("name", "Hot Bakery Cafe")
    .append("stars", 4)
    .append("categories",
      asList("Bakery", "Cafe", "Coffee", "Dessert")),
  new Document("name", "XYZ Coffee Bar")
    .append("stars", 5)
    .append("categories",
      asList("Coffee", "Cafe", "Bakery", "Chocolates")),
  new Document("name", "456 Cookies Shop")
    .append("stars", 4)
    .append("categories",
      asList("Bakery", "Cookies", "Cake", "Coffee")));

collection.insertMany(documents);
```

5 example `documents` are being inserted into the `restaurantscollection`. Each `document` represents a restuarant with a name, star rating, and categories (stored as an array).

看来`Document`对应着表项，`Collection`对应着表

名为`test`的数据库中包含着一张名为`resturants`的表，一个数据库应该可以包含多张表

## Create a query ##

```java
// 3. Query 
List<Document> results = collection.find().into(new ArrayList<>());
```

In this example, we run a simple query to get all of the `documents` in the restaurants `collection` and store them as an array.

## Build an index ##

```java
// 4. Create Index 
collection.createIndex(Indexes.ascending("name"));
```

Indexes in `MongoDB` are similar to indexes in other `database` systems. `MongoDB` supports indexes on any field or sub-field of a `document` in a `collection`. Here, we are building an index on the name field with sort order ascending.

`MongoDB`是否支持多项联合排序？

不过如果用`MongoDB`的目的是为了建立JSON树，排序的需求是很低的，应该用简单排序就可以搞定

## Aggregate ##

```java
// 5. Perform Aggregation
collection.aggregate(asList(match(eq("categories", "Bakery")),
  group("$stars", sum("count", 1))));

mongoClient.close();
```

Using `MongoDB`’s aggregation pipeline, you can filter and analyze data based on a given set of criteria. In this example, we pull all the `documents` in the restaurants `collection` that have a category of Bakery using the \$match operator and then group them by their star rating using the \$group operator. Using the accumulator operator, \$sum, we can see how many bakeries in our `collection` have each star rating.

`Aggregate`在我现在的理解中，就是对数据的处理与分析，而且处理过程有点类似于`MapReduce`
