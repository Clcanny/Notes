import Prelude hiding (length, null, (++), concat, all, any, and, or, reverse, splitAt, take, drop, head, tail, last, init, words, lines)

length [] = 0
length (x:xs) = length xs + 1

null [] = True
null _ = False

(++) [] ys = ys
(++) (x:xs) ys = x:(xs ++ ys)

concat [] = []
concat (x:xs) = x ++ (concat xs)

all f [] = True
all f (x:xs) = (f x) && (all f xs)

any f [] = False
any f (x:xs) = (f x) || (any f xs)

and xs = all id xs

or xs = any id xs

reverse [] = []
reverse (x:xs) = (reverse xs) ++ [x]

splitAt n xs
    | n < 0 = ([], [])
    | n == 0 = ([], xs)
splitAt _ [] = ([], [])
splitAt n (x:xs) = (x:first, second)
  where (first, second) = splitAt (n - 1) xs

take n xs = fst (splitAt n xs)

drop n xs = snd (splitAt n xs)

head [] = error ""
head (x:xs) = x

tail xs = drop 1 xs

last xs = head (reverse xs)

init xs = reverse (tail (reverse xs))

safeHead [] = Nothing
safeHead xs = Just (head xs)

safeTail [] = Nothing
safeTail xs = Just (tail xs)

safeLast [] = Nothing
safeLast xs = Just (head xs)

safeInit [] = Nothing
safeInit xs = Just (init xs)

splitWith pred (x:xs)
  | pred x = splitWith pred xs
  | otherwise = first:(splitWith pred rest)
      where (first, rest) = break pred (x:xs)
splitWith _ [] = []

words = splitWith (\x -> x == ' ' || x == ',' || x == ';')

lines = splitWith (\x -> x == '\r' || x == '\n')
