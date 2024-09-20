
bool riscaldamento ( float sp, float pv, float hyst, bool _en){
    bool state = false;
    int limitSec = 3600; /// 1 orA
    ///Serial.println(limit);
    if (_en) {
          if (pv < (sp+hyst)){ // da scaricare 
              state = true;
              limit++;
              if (limit > limitSec)
              {
                state = false; // se la temepratura non è ancora sarita dopo un oera spengo a prescidere
              } else
              {
                state = true;
              }
              if (! flag1){
                Serial.print(pv); Serial.print(" "); Serial.print(sp); Serial.print(" "); Serial.print(sp - hyst); Serial.println(" ACCESO");
                flag1 = true;
              }
              flag2 = false;
          }
          else if (pv > (sp - hyst)){ /// da modificare
            if (!flag2){
              Serial.print(pv); Serial.print(" "); Serial.print(sp); Serial.print(" "); Serial.print(sp + hyst);Serial.println(" sPENTO");
              flag2 = true;
            }
            flag1 = false;
            state = false;
            limit = 0; /// azzero il conteggio
            
          }

    }
    else
    {
      limit = 0;
      state = false;
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

