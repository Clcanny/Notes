data BookInfo = Book Int String [String]
                deriving (Show)

myInfo = Book 9780135072455 "Algebra Of Programming" ["Richard Bird", "Oege de Moor"]

bookID (Book id _ _) = id
bookTitle (Book _ title _) = title
bookAuthors (Book _ _ authors) = authors

id = bookID myInfo
title = bookTitle myInfo
authors = bookAuthors myInfo
