myDrop n xs = if n <= 0 || null xs
              then xs
              else myDrop (n - 1) (tail xs)

anotherDrop 0 xs = xs
anotherDrop n [] = []
anotherDrop n xs = anotherDrop (n - 1) (tail xs)

