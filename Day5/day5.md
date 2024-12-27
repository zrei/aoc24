<h1>Part 1</h1>
First, build an adjacency list.

Then, use two for loops to iterate through each line, checking each entry with the entries that come after it to determine if the positions are correct.

<h1>Part 2</h1>
First identify the invalid lines with part 1's code.

Then, we can sort the numbers in each line by their number of "dependencies", where the dependencies are the number of times it must appear before another entry. Numbers with higher dependencies should occur earlier to avoid conflicts.