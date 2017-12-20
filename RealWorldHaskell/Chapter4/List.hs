import Prelude hiding (length, null, (++), concat, all, any, and, or, reverse, splitAt, take, drop, head, tail, last, init)

data List a = Cons a (List a)
              | Nil
                deriving (Show)

fromList [] = Nil
fromList (x:xs) = Cons x (fromList xs)

toList Nil = []
toList (Cons x xs) = x : (toList xs)

length Nil = 0
length (Cons x xs) = length xs + 1

null Nil = True
null _ = False

(++) Nil ys = ys
(++) (Cons x xs) ys = Cons x (xs ++ ys)

concat Nil = Nil
concat (Cons x xs) = x ++ (concat xs)

all f Nil = True
all f (Cons x xs) = (f x) && (all f xs)

any f Nil = False
any f (Cons x xs) = (f x) || (any f xs)

and xs = all id xs

or xs = any id xs

reverse Nil = Nil
reverse (Cons x xs) = (reverse xs) ++ (Cons x Nil)

splitAt n _
    | n <= 0 = (Nil, Nil)
splitAt _ Nil = (Nil, Nil)
splitAt n (Cons x xs) = (Cons x first, second)
  where (first, second) = splitAt (n - 1) xs

take n xs = fst (splitAt n xs)

drop n xs = snd (splitAt n xs)

head (Cons x xs) = x
head Nil = error ""

tail xs = drop 1 xs

last xs = head (reverse xs)

init xs = reverse (tail (reverse xs))
