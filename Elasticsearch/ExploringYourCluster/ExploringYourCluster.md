# Exploring Your Cluster #

## The REST API ##

Now that we have our node (and cluster) up and running, the next step is to understand how to communicate with it. Fortunately, Elasticsearch provides a very comprehensive and powerful REST API that you can use to interact with your cluster. Among the few things that can be done with the API are as follows:

+ Check your cluster, node, and index health, status, and statistics
+ Administer your cluster, node, and index data and metadata
+ Perform CRUD (Create, Read, Update, and Delete) and search operations against your indexes
+ Execute advanced search operations such as paging, sorting, filtering, scripting, aggregations, and many others

## Cluster Health ##

Let’s start with a basic health check, which we can use to see how our cluster is doing. We’ll be using curl to do this but you can use any tool that allows you to make HTTP/REST calls. Let’s assume that we are still on the same node where we started Elasticsearch on and open another command shell window.

