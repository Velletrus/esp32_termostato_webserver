int risc_state = 0;
bool state = false;

bool riscaldamento ( float sp, float pv, float hyst, bool _en){
    
    int limitSec = 3600; /// 1 orA
    ///Serial.println(limit);
    if (_en) {
          switch (risc_state){
            case 0 :
             if (sp > (pv + hyst))
                {  
                  state = true;
                  risc_state = 10; // riscaldamento
                  limit++;
                  Serial.print(pv); Serial.print(" "); Serial.print(sp); Serial.print(" "); Serial.print(sp - hyst); Serial.println(" ACCESO");
                }
            break;
            case 10:
               if (pv > (sp + hyst))
                  {
                    state = false;
                    risc_state = 20;// entro in isteresi
                    Serial.println("hyst in");
                    Serial.print(risc_state); Serial.print(" ");Serial.print(pv); Serial.print(" "); Serial.print(sp); Serial.print(" "); Serial.print(sp + hyst);Serial.println(" sPENTO");
                  } 
            break;
            case 20: 
                limit = 0;
                if (sp < (pv - hyst)) // esco isteresi
                {
                  risc_state = 0;
                  Serial.println("hyst out");
                }
            break;

          }

    }
    else
    {
      limit = 0;
      state = false;
      flag1 = false;
      flag2 = false;
      risc_state = 0;

    }
    /// conteggio secondo per forzare spegimento se non è arrivato a temperatura
    
    digitalWrite(caldaia, state);
    return state;

}
void simulazione(){
  /*if (updn){
    sym = sym + 0.3;
  }else{
    sym = sym - 0.3;
  }
  if (t < 15){
    updn = true;
  }else if ( t > 32) {
    updn = false;
  }*/
  ;
}

