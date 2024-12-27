<h1>Part 1</h1>
Used recursion with caching to see if the end of the towel could be reached.

I realise now that you could probably DP this by starting from the back of the towel instead.

<h1>Part 2</h1>
Modified the part 1 code to return an integer instead of a boolean, still using caching.

Had some overflow previously becaused I was using an `unsigned long` instead of an `unsigned long long`.