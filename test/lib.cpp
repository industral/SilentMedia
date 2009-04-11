#include <iostream>

// programmed action interface ( PAI )

int main ( void ) {
   Action * action = Action::Instance();
   std::string actionName;
   
   actionName = "shortgun";
   action -> addNew ( actionName );
   
   
   action -> addFile ( "fire.wav", "fire" );
   action -> addFile ( "shells.wav", "shells", "fire" );
   action -> addFile ( "impact.wav", "impact", "shells" );
   
   action -> addFile ( "fire.wav", "fire" );
   action -> setParams ( "fire", 0 );
   action -> addFile ( "shells.wav", "shells", "fire" );
   action -> setParams ( "shells", 1000 );
   action -> addFile ( "impact.wav", "impact", "shells" );
   action -> setParams ( "impact", 500 );
   
   
   std::string __fire = action -> addFile ( "fire.wav", NULL, NULL );
   action -> setParams ( __fire, 0 );
   std::string __shells = action -> addFile ( "shells.wav", __shells, __fire );
   action -> setParams ( __shells, 1000 );
   std::string __impact = action -> addFile ( "impact.wav", __impact, __shells );
   action -> setParams ( __impact, 500 );
   
   //
   action -> addNew ( "plasmaGun" );
   action -> addFile ( "beginFire.wav", "begin" );
   action -> addFile ( "fire.wav", "fire" );
   action -> addFile ( "fly.wav", "fly", "fire", SML_BREAKPOINT );
   action -> addFile ( "end.wav", "end", NULL );
   action -> addAction ( "crash.wav", "crash", NULL, "crash" );
   action -> apply();
   
   action -> call ( "plasmaGun" );
   if ( ... ) {
      action -> call ( "plasmagun", "crash", SML_CONTINUE );
   }
   //
   
   action -> apply ( actionName );
   action -> cancel ( actionName );
   
   
   