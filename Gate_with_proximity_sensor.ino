int state=0;
boolean swt1=0;
boolean swt2=0;
char data;
boolean sense=1;
boolean act=0;


void setup()
{
  Serial.begin(9600);
  pinMode(2,INPUT_PULLUP);
  pinMode(3,INPUT_PULLUP);
  pinMode(9,INPUT_PULLUP);
  pinMode(5,OUTPUT);
  pinMode(7,OUTPUT);
  Serial.println("Start");
}

void loop() 
{
  Serial.print("Enter the key in caps : ");
  data=Serial.read();
  Serial.println(data);
  prog();
  delay(500);
}

void prog()
{
  if(data=='O')
  {
    opn();
  }
  else if (data=='H')
  {
    hold();
  }
  else if (data=='C')
  {
    cls();
  }
  else
  {
    stay();
  }
}
void opn()
{
  if(state==0)//already open
  {
    stay();
    Serial.println("Open already.");
  }
  else if(state==2)//was closed
  {
    state=1;
    Serial.println("Opening..");//status=opening
    swt1=digitalRead(2);
    while(swt1!=0)
    {
      swt1=digitalRead(2);
      data=Serial.read();
      if(swt1==0)
      {
        swt1=digitalRead(2);
        Serial.println("switch pressed..");
        break;
      }
      else if(data=='C')
      {
        state=3;
        cls();
        act=1;
        break;
      }
      else if(data=='H')
      {
        hold();
        break;
      }
      clockwise();
    }
    state=0;//status set to opened
    Serial.println("Open");
  }
  else if(state==1)
  {
    Serial.println("Was opening..");
    swt1=digitalRead(2);
    while(swt1!=0)
    {
      swt1=digitalRead(2);
      data=Serial.read();
      if(swt1==0)
      {
        swt1=digitalRead(2);
        Serial.println("switch pressed..");
        break;
      }
      else if(data=='H')
      {
        hold();
        break;
      }
      clockwise();
    }
    if(act=1)
    {
    state=2;
    }
    else
    {
      state=0;
      Serial.println("Open");
    }
  }
  else
  {
    stay();
  }
}

void cls()
{
  if(state==2)//already closed 
  {
    stay();
    Serial.println("Closed  already.");
  }
  else if(state==0)//was open
  {
    state=3;
    Serial.println("Closing..");//status=closing
    swt2=digitalRead(3);
    while(swt2!=0)
    {
      sense=digitalRead(9);
      swt2=digitalRead(3);
      data=Serial.read();
      if(swt2==0)
      {
        swt2=digitalRead(3);
        Serial.println("switch pressed..");
        break;
      }
      else if(sense==0)
      {
        Serial.println("Proximity sensor activated..");
        stay();
        delay(1000);
        state=1;
        act=1;
        opn();
        break;
      }
      else if(data=='O')
      {
        state=1;
        opn();
        act=1;
        break;
      }
      else if(data=='H')
      {
        hold();
        break;
      }
      anticlockwise();
    }
    if(act==1)
    {
      state=0;
    }
    else
    {
      state=2;//status set to closed
      Serial.println("Close");
    }
  }
  else if(state==3)
  {
    Serial.println("Was closing..");
    swt2=digitalRead(3);
    while(swt2!=0)
    {
      swt2=digitalRead(3);
      data=Serial.read();
      if(swt2==0)
      {
        swt2=digitalRead(3);
        Serial.println("switch pressed..");
        break;
      }
      else if(data=='H')
      {
        hold();
        break;
      }
      anticlockwise();
    }
    state=2;//status set to closed
    Serial.println("Close");
  }
  else
  {
    stay();
  }
}

void hold()
{
  if(state==0)
  {
    stay();
    Serial.println("Fully opened.");
  }
  else if(state==2)
  {
    stay();
    Serial.println("Fully closed.");
  }
  else if(state==1)
  {
    Serial.println("Was opening..");
    while((data!='K')||(data!='C'))
    {
      stay();
      data=Serial.read();
      if(data=='K')
      {
        opn();
        break;
      }
      else if(data=='C')
      {
        state=3;
        cls();
        break;
      }
     }
  }
  else if(state==3)
  {
    Serial.println("Was closing..");
    while((data!='K')||(data!='O'))
    {
      stay();
      data=Serial.read();
      if(data=='K')
      {
            cls();
            break;
      }
      else if(data=='O')
      {
        state=1;
        opn();
        break;
      }
    }
  }
}
void stay()
{
  digitalWrite(5,0);
  digitalWrite(7,0);
}
void clockwise()
{
  digitalWrite(5,1);
  digitalWrite(7,0);
  Serial.println("Clockwise rotation.."); 
}
void anticlockwise()
{
  digitalWrite(5,0);
  digitalWrite(7,1);
  Serial.println("Anticlockwise rotation..");
}
