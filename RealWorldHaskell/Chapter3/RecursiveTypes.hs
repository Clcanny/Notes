data List a = Cons a (List a)
            | Nil
              deriving (Show)

-- data VarList a b = VarCons a (VarList b)
--                  | VarNil
--                    deriving (Show)

data Tree a = Node a (Tree a) (Tree a)
            | Empty
              deriving (Show)

fromList (x:xs) = Cons x (fromList xs)
fromList [] = Nil

darlingVivian = fromList "darlingVivian"
