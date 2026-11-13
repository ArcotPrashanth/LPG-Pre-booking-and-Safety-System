void displayText(String str,int x,int y)
{
  display.setCursor(x,y);             // Start at top-left corner
  display.println(str);  
  display.display();
}
