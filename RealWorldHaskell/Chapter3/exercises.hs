-- myLength :: [a] -> Int
-- myLength [] = 0
-- myLength x:xs = myLength xs + 1

mean [] = 0
mean xs = fromIntegral (sum xs) / fromIntegral (length xs)

palindrome xs = xs ++ (reverse xs)

isPalindrome xs = xs == (reverse xs)

intersperse :: a -> [[a]] -> [a]
intersperse _ [] = []
intersperse _ (y:[]) = y
intersperse x (y:ys) = y ++ (x:(intersperse x ys))

data Tree a = Node a (Tree a) (Tree a)
            | Empty
              deriving (Show)

height Empty = 0
height (Node x left right)
    | lengthOfLeft >= lengthOfRight = lengthOfLeft + 1
    | True = lengthOfRight + 1
  where lengthOfLeft = height left
        lengthOfRight = height right

data Point = Point Int Int deriving (Eq, Show)

data Direction = TurnLeft | StraightLine | TurnRight deriving (Eq, Show)

calculateDirection :: Point -> Point -> Point -> Direction
calculateDirection (Point x1 y1) (Point x2 y2) (Point x3 y3)
  | crossProductDirection > 0 = TurnLeft
  | crossProductDirection < 0 = TurnRight
  | otherwise                 = StraightLine
    where
      crossProductDirection = ((x2 - x1) * (y3 - y1)) - ((y2 - y1) * (x3 - x1))

tripleDirections :: [Point] -> [Direction]
tripleDirections (p1:(p2:(p3:ps))) = (calculateDirection p1 p2 p3) : tripleDirections (p2:(p3:ps))
tripleDirections _ = []
