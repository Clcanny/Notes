data Color = Yellow | Green | Red
             deriving (Show)

type CardHolder = String
type CardNumber = String
type Address = [String]
type CustomerId = Int

data BillingInfo = CardNumber CardHolder Address
                 | CashOnDelivery
                 | Invoice CustomerId
                   deriving (Show)
