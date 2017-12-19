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

deleteEnter ('\r':'\n':rest) = rest
deleteEnter ('\r':rest) = rest
deleteEnter ('\n':rest) = rest
deleteEnter all = all

main = mainWith (\x -> concat $ splitLines x)
  where mainWith function = do
          args <- getArgs
          case args of
            [input, output] -> interactWith function input output
            _ -> putStrLn "error: exactly two arguments needed"
