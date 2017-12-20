-- docker run -t -i -v `pwd`:/test --rm haskell sh -c
-- "cd test && runghc InteractWith.hs helloworld.in helloworld.out"
import System.Environment (getArgs)

interactWith function inputFile outputFile = do
  input <- readFile inputFile
  writeFile outputFile (function input)

splitLines [] = []
splitLines cs = pre : splitSuf
  where (pre, suf) = break isLineTerminator cs
        splitSuf = splitLines (deleteEnter suf)
        isLineTerminator c = c == '\r' || c == '\n'
        deleteEnter input = case input of
          ('\r':'\n':rest) -> rest
          ('\r':rest) -> rest
          ('\n':rest) -> rest
          all -> all

main = mainWith (\x -> unlines $ getFirstWordsA x)
  where mainWith function = do
          args <- getArgs
          case args of
            [input, output] -> interactWith function input output
            _ -> putStrLn "error: exactly two arguments needed"

getFirstWords str = firstWords
  where lines = splitLines str
        firstWords = getFirstWords' lines
        getFirstWords' lines = case lines of
          [] -> []
          (x:xs) -> (head (words x)):(getFirstWords' xs)

getFirstWordsA = map head . map words . splitLines
