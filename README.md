# BlazeX

## Keep it simply simple

- Worker process management
	- Fork multiple workers as needed
	- Detach worker from its controlling terminal
- Shared memory management
- Flood routing algorithm for inter-process communication, can emulate broadcast, unicast, multicast messages
- Main thread works with read/write IO
- Thread pool management
	- Threads are created on the fly until WORKER_THREAD_TRESHOLD is reached
	- When reached, jobs are queued and when a thread terminates, it will notify the master pool thread that it can execute the task
- Any runtime thread that needs to work with IO MUST only __notify__ (as main thread is running inside libev main loop, notify means make the event pending) the main IO thread that data are available for writing and might also re-order the pending IO events to make write event have a highest priority.

Notification are done within a job thread, it invokes a specific method that adds the bytes to the buffer associated to a route and then make this route active until there are bytes to consume. event_add should be thread safe as many workers would concurrently need to push some IO jobs.


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
```
pid_file=/var/run/blazeX.conf

[workers]
nb_processes: 10
max_worker_threads_per_process: 1

```

# Administration

##  CLI Management console (readable via distinct FD eg: net socket, unix_socket) 
* Listing available buckets
	
	_Format_: "action"
	
```
	blx> blist
	campaigns
	publishers
```
	
* Defining a new bucket (updating a bucket structure)

	_Format_: "action";"null char";"bucket";"file";

```
	blx> define;\N;bucket;;
 	blx> field_name; field_type; field_size; field_default;
 	blx> field 
 	blx> \.
```
* Copying data to a bucket

Copying action has 2 forms. The 1st one copies data from the terminal to the bucket in which case you need to indicate the end of data using \.. 

	_Format_: "action";"null char";"bucket to add data to";"field list expression";"file";
	
```
	blx> copy; bucket;\N;field1,field2;;
	blx> .... 
	blx> \.

	blx> copy; bucket; \N; field1,field2; file.csv;
```
* Reading data from a bucket

	_Format_: "action"; "bucket"; "select_list"; "expression"
	
```
	blx> map;campaigns;campaign_name,active;"campaign_name ~ 'zyn.*' and active = true "
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
	_Format:_ "action"; "bucket_to_update";  "filter_expr"; "update_expr"
```
	blx> update;campaigns;active=true;active=false;
```

* Dumping a bucket

Dumping a bucket does not dump bound data with user-defined bindings. Instead, it will contain the commands
used to define the bucket structure, re-insert the data, and if available re-create the bindings


	_Format:_ "dump"; "bucket"; "select_list"; file.csv;
	
```
	blx> dump;campaigns;campaign_name,active; dump.csv
	blx> define;\N;bucket;;
	...
	blx> \.
	blx> copy ...
	....
	blx> \.
	blx> bind; ...
```	

* Aggregates

BlazeX is able to aggregate data based on an assignement expression.

	_Format_: aggregate; bucket; declare_expr; "agg_expr"; "end_expr"; "group_expr"

Where:  
	- bucket defines the main bucket that will be aggregated
	- declare_expr declares request variable to store aggregates result
	- agg_expr defines an expression on which to compute aggregate
	- end_expr defines a one-time computation and has access to variable declared on this request scope
	- group_expr defines the group of field on which to compute aggregate
	
```
# Case 1: count distinct name
	aggregate; students; "count=0"; "count += 1";;"students.name"
# Case 2: cout average note by students
	aggregate; students; "count=0; avg=0"; "count += 1; notes += students->examination.note";" avg= notes/count"; "students.name" 
```

* List bindings

* Defining a binding

	_Format_: action;bind target;bind source;bind expr;

```
	blx> bind;campaigns;publishers;"(campaign.name = 'zynga' and publisher.category ~ '.*game') or (campaign.name = 'test' and true)" 
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

```
 
## Shared memory internal
[See shm usage](http://www.cs.cf.ac.uk/Dave/C/node27.html)

## Query language definition
Query language **MUST** rely on a simple form to reduce parsing time. As all we need to store can fit into a tabular format, we will stick to CSV.

It should be make easy to compute an aggregate on a group of fields, to filter a list, to compute max, avg, min of a field.

## "sum"; "s += 1";field1, field2;
k = key(field,field2);
// hashmap[key];

A query response **MUST AVOID** copying the original when streaming back the result. In the worst case, it should **ONLY** maintain a pointer to the orignal node in the list containing the bytes
to be streamed. **Note** that some query might return data that are not taken from a node's content, therefor, the data must be copied and the copy must be kept until it gets streamed.


### How can we interact with a data set ?

Each dataset can be queried using a string expression which (by the use of [Shunting Yard Algorithm] (https://en.wikipedia.org/wiki/Shunting-yard_algorithm) ) will be transformed into a traversable output queue and interpreted.

Supported operators :
- op1 **~** op2  : Returns true if op1 matches the regexp op2
- op1 **and** op2 
- op1 **or** op2 
- op1 **==** op2
- op1 **+** op2
- op1 **/** op2
- op1 **x** op2
- var **=** expr
- ? ( boolean_expr: expr ) : Evaluates expr only if boolean_expr returns true

## Bench

## Resources
[ GNU libc ] (http://www.gnu.org/software/libc/manual/html_node/index.html#SEC_Contents)
[ Is Parallel Programming Hard, And, If So, What Can You Do About It? ] (https://www.kernel.org/pub/linux/kernel/people/paulmck/perfbook/perfbook.2015.01.31a.pdf)
[ Thread wait/notify using phread ] (http://maxim.int.ru/bookshelf/PthreadsProgram/htm/r_28.html)





