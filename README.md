# BlazeX

## Overview

- BlazeX binary is designed to work asynchronously with  I/O events at a process level. Once an I/O event is considered completed, it translates the event into a set of organized jobs able to be dispatched and synchronize against multiple threads.
- Each process can hold its own set of events
- When an operation occurs on an event, the event stream parser takes care of translating the input data stream into a global symbol map. Suppose you receive an HTTP request with http:<host>?a=1&b=bob (the symbol table should expose var a type int with 1 as its value and b of type char*(16) with "bob" as value. This process is called the extract/transform process or data mapping.
- A map is local to an event processing scope (scope of symbols should not bypass process isolation). Another event should not altered in any way the symbols added by another event
 - After an event has finished its mapping step, it will likely to try to retrieve a result based on its input
 - The event is given to the "Executor Manager" which translates it into a bunch of jobs and manage their execution, and also their synchronization accross the thread group available for that process
 - A job is required to be splitted into ordered individual task
  - point out a list or a set of list to read (if more than 1 list a join is performed)
  - optionally filter or map nodes based on the result set
  - optionally aggregate any result
  - optionally sort result
- Asynchronous multi-process worker with task_queue(); and worker that executes jobs from task queues when available (a job is a either a map, reduce, sort execution chain). A job belons to a group, a group of job can be scaled at thread-level


A field structure maintains a max size fixed at compile time
- Supported field structure data types: 
- Support binding between sets. A binding is expressed using a "binding expression" attached to a list inside a bucket, when the expression matches a pointer to the node is created inside the origin node. Suppose, you have a set of campaigns (which contains field of type: name:string, is_active:short). Publishers have a filtered inventory of campaign, then you could just
add each publishers for each campaign node into a field of type array, or more easily you add all your publishers into a distinct set and define a binding expression that will create for each campaign node a list of pointers to each publishers mapping the expression.


# Logging features
* Uses syslog as daemon logging

# Configuration


# Administration

##  CLI Management console (readable via distinct FD eg: net socket, unix_socket) 
* Listing available buckets
	
	_Format_: "action"
	
```
	blx> bucket_list
	campaigns
	publishers
```
	
* Defining a new bucket (updating a bucket structure)
```
	blx> define;campaigns;'{name:"campaign_name", type: string, size: 16 },{ name:"active", type: boolean, size: 8 }'
 	blx> define;publishers;'{name:"pub_name", type: string, size: 16 }'
 	blx> r:ok
```
* Pushing data to a bucket

	_Format_: "action";"bucket to add data to";"field list expression";
	
```
	blx> add;[ { campaign_name: "zynga", active: true }, { campaign_name: "uber", active: false }, { campaign_name: "zynna", active: true } ]
```
* Reading data from a bucket
```
	blx> map;campaigns;"campaign_name ~ 'zyn.*' and active = true "
	blx> 
	campaign_name; active;
	"zynga";"true"
	"zynna";"true"
```
* Removing data from a bucket

	_Format_: "action"; "bucket"; "expression"
	 
```
	blx> "rm";"campaigns";"campaigns.active = false";
```
* Updating a set of field_aggregate
	** Format: "action"; "bucket_to_update";  "filter_expr"; "update_expr"
```
	blx> update;campaigns;active=true;active=false;
```

* List bindings

* Defining a binding

	_Format_: "action";"bind target";"bind source";"bind expression";

```
	blx> # For each publisher that have a ranking > 0, bind any campaign
	blx> bind;campaigns;publishers;"campaign.name = 'zynga'"; "publisher.type 
```
# In-memory database storage

```c
typedef unsigned short BLX_DATA_FIELD_TYPE_STRING 0;
typedef unsigned short BLX_DATA_FIELD_TYPE_INT_16 1;
typedef unsigned short BLX_DATA_FIELD_TYPE_INT_32 2;
typedef unsigned short BLX_DATA_FIELD_TYPE_XXX

struct blx_db_data_field {
	char name[16];
	unsigned short blx_db_field_type;
	unsigned int blx_db_field_size; 
};

struct blx_data_structure {
	struct blx_data_field** data_fields; // Set of data fields that a bucket might reference 
};

struct blx_db_bucket  {
	char name[16];
	struct blx_db_data_structure data_structure;
	DoubledLinkedList data_list;
	struct blx_db_data_binding data_binding;	
};

struct blx_db_data_field_value {
	void** data; // List of pointers to fields value, fields are sized-aligned. To read field, we must known the field size
	
}
 // Returns a pointer to the fields value with its size 
void* blx_db_read_data_field(struct blx_db_bucket* bucket, struct blx_db_data_field_value* data_field_value, struct blx_language_expr expr){
}


``̀
 
## Shared memory internal
[See shm usage](http://www.cs.cf.ac.uk/Dave/C/node27.html)



## Query language definition
- A binary tree structure is used to compile the char* expression string. Binary tree is then traversed, result is stored and dispatched to any I/O routes that expect to write back the result data.
- The result data MUST NOT BE a copy of the reference bucket data set. Make simply the result list be a set of pointers to the referenced dataset.
 For a simple list of 4 millions element, we would have sizeof(list) + 4m elements * sizeof(void*) => 3906 Kb => 3.8 Mb to copy a large result set. More generally speaking a result set would have to store more than 1 pointer (so the size would double or triple depending on how many bindings a set has)

This makes each copy of, query on, a dataset have a limited size can be be expressed

The memory allocator should allocate memory or take care of these components :
 - Result storage segment
 - Runtime segment (all but result storage segment) 

- A query must be "read simple" to reduce parsing time, it should stick to a simple inline format with a field separator
  - eg: point CAMPAIGNS;  (list.campaign_id > 12 and list.campaign_name ~ /$bob/ and list.publishers.category ~ '/cat/');;list.name asc;
  - eg: list.boolean = true 

## Bench
A structure set of type { int f1; char* field2 (16 char  long); } containings 10^9 elements 
would result in an in-memory size of : (10^9) * 20 + 5 =  953.67 Mbytes = 9,4Gb



