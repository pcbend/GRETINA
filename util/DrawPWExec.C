
void DrawPWExec() {
  int event = gPad->GetEvent();


  if((event!=kKeyPress) && (event!=0x00000001)) 
     return;

  int px = gPad->GetEventX();
  int py = gPad->GetEventY();


  //printf("key e = 0x%08x\n",event);
  //printf("key x = 0x%08x\n",px);
  //printf("key y = 0x%08x\n",py);

  if(event==0x00000001) {
      gApplication->Terminate();
  } else if(px == 0x00000020) {
      gApplication->Terminate();
  }
  return;
}
