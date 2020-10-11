

**single stock market**

components

processes - 
- stex engine - takes offers, produces values and deals
- offer generator - takes values, produces offers
- value server

resources
- offers mqueue
- value updates mqueue

flow - 
1. stex engine waits for offers; value server listens for requests
2. fire up the generator - request current values then push offers to mqueue
3. engine reads these, copies to internal data structure then matches
4. if a match was found, engine will produce a deal and push the updated value to mqueue

things to keep in mind
- types of offers - how to prioritize and when to match / execute
- multiple stocks - how long before RAM runs out
- persistency - ?



