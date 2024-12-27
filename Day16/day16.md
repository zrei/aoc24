<h1>Part 1</h1>
Used Dijkstra for this part and had to remember how to use a priority queue in CPP. :P

<h1>Part 2</h1>
Ran part 1 first to get the minimum cost.

Used dijkstra again, but using a struct that represents a point + direction that points towards the previous point + direction on the path as well. To avoid too large a number of possibilities, the points were pruned with the following rules:
* If the overall cost has exceeded the minimum cost, we can stop
* If the cost to reach this point + direction exceeds the previously recorded minimum cost, we can stop (there is an unordered map recording the minimum cost to reach a point + direction which will be the first time we reach that point + direction by dijkstra's rules)
* If there is a known cost from this point + direction to the end point and the sum of the known cost with the current cost is greater than the minimum cost, we can stop (the known cost is recorded when tracing a path that has reached the end to find the unique points on it)