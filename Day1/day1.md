<h1>Part 1</h1>
Fairly simple, just read each number on a single line into two different vectors, then sorted both. Iterated through the sorted lists to find the absolute difference between each entry.

<h1>Part 2</h1>
Could have just stored the second list's numbers in an unordered map mapping to the number of entries, but I decided to do a singe O(n) iteration through both sorted lists.

It moves the pointer on the second list until it reaches a number greater than the pointed to number on the first list, noting the number of entries that are the same. It then advances the pointer on the first list until a new number is reached, adding the occurrences at the same time.