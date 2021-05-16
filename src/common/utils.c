unsigned int atoi(char *string)
{
  unsigned int res;
  while(*string)
  {
    res = res * 10 + (*string) - '0';
    string++;
  }
  return res;
}
