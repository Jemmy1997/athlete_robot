# sbe306-assignment3-team-14-1 . (**Gym**)
| Team Members | SEC | BN |
|--------------|-------------|---------|
| **Ahmed Gamal**  | 1 |1|
| **Hazem Muhammad**| 1 | 32 |
| **Sohail Ahmed** | 2 | 1|
|**Mohamed Gamal** |2 |20|

#### We made a graphic design about gym and made 2 animations using timer function (**glutTimerFunc()**) and Swap Buffer (**glutSwapBuffers()**).
Example
```sh
void timer(int value)
{
int l=300;
int l2=300;
    if (value == 1)
    {    
      if (shoulder2 < 90)
      {
         shoulder2 = (shoulder2 + 5) % 360;
		     glutPostRedisplay();
         glutTimerFunc(l, timer, 1);
      }
	  if  (rshoulder2 < 180)
	  {
		rshoulder2 = (rshoulder2 + 5) % 360;
		glutPostRedisplay();
		glutTimerFunc(l, timer, 1);
	  }
	  else if  (elbow2 > -120)
	  {
		 
		elbow2 = (elbow2 - 5) % 360;
		glutPostRedisplay();
		glutTimerFunc(l, timer, 1);
		
	  }
	  
    else 
	  glutTimerFunc(l, timer, 2);
	}
}	
```





![First_animation](https://github.com/sbme-tutorials/sbe306-assignment3-team-14-1/blob/master/57511480_315409972439807_5190963272844574720_n.png?raw=true)
![first_animation](https://github.com/sbme-tutorials/sbe306-assignment3-team-14-1/blob/master/56806559_417753335452231_5305956211831603200_n.png?raw=true)


