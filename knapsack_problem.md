# knapsack problem
## Background
    In our company, we choose a match algorithm in the game. At last, we think that the knapsack problem is suitable for this situation.
## Describe
    There is a bag that it can contain M weight item at most.And those items have the own weight and the worth.The problem is how to make the promise that the bag has the most worth.
## Psedocode
```
Input: Two sequences v=<v1,...,vn>, w=<w1,...,wn>, the number of items n and the maxinum weight W.
Output: The optimal value of the knapsack
for w <- 0 to W do
  S[0, w] <- 0;
end for
for i <- 1 to n do
  S[i, 0] <- 0;
  for w <- 1 to W do
    if wi <= w then
      if vi + S[i-1, w-wi] > S[i-1, w] then
        S[i, w] <- vi + S[i-1, w-wi];
      else
        S[i, w] <- S[i-1, w];
      end if
    else
 		  S[i, w] <- S[i-1, w];
    end if
  end for
end for
return S[n, W];
```
