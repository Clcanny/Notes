import Data.Char (digitToInt, isDigit)
import Data.List (foldl')
import Prelude hiding (takeWhile, any)

asIntMaybe :: String -> Maybe Int
asIntMaybe ('-':str) = case (asIntMaybe str) of
  Nothing -> Nothing
  Just x -> Just (-1 * x)
asIntMaybe str = foldl' step (Just 0) str
  where step x y = case x of
          Nothing -> Nothing
          Just x -> case (isDigit y) of
            True -> Just (10 * x + digitToInt y)
            otherwise -> Nothing

asIntEither :: String -> Either String Int
asIntEither ('-':str) = case (asIntEither str) of
  Right x -> Right (-1 * x)
  l -> l
asIntEither str = foldl' step (Right 0) str
  where step x y = case x of
          Right x -> case (isDigit y) of
            True -> Right (10 * x + digitToInt y)
            otherwise -> Left ""
          l -> l

myConcat :: [[a]] -> [a]
myConcat = foldl (++) []

takeWhile cond xs = foldr step [] xs
  where step x zero = case (cond x) of
          True -> x:zero
          False -> []

any cond xs = foldr step False xs
  where step x zero = (cond x) || zero
