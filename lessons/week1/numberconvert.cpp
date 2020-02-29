// Reference
// http://codeofthedamned.com/index.php/number-base-conversion


#include <iostream>

// The set of possible symbols for representing other bases.

const char symbols[] = {'0', '1', '2', '3', '4', '5', '6', '7', 
                        '8', '9', 'A', 'B', 'C', 'D', 'E', 'F', 
                        'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 
                        'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 
                        'W', 'X', 'Y', 'Z'};

void ConvertToBase( const unsigned long decimal,
                    const unsigned long radix)
{
  unsigned long remainder[32] = {0};
  unsigned long quotient = decimal;
  unsigned char place    = 0;

  while (0 != quotient)
  {
    unsigned long value = quotient;

    remainder[place] = value % radix;
    quotient = (value - remainder[place]) / radix;
    place++;
  }

  std::cout << decimal << " in base " << radix << " is ";

  for (unsigned char index = 1; index <= place; index++)
  {
    std::cout << symbols[remainder[place - index]];
  }
  std::cout << '\n';
}

void main()
{

  ConvertToBase(237,        2);
  ConvertToBase(237,        10);
  ConvertToBase(237,        16);
  ConvertToBase(3134243038, 16);
  ConvertToBase(3134243038, 36);
}