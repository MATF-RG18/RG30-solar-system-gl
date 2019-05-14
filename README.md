# RG30-solar-system-gl
Solar System GL

Projekat je zamisljen kao simulacija nastanka Suncevog sistema. Objekti simulacije su Sunce, planete, sateliti i komete.

Nakon pocetne eksplozije i nastanka Sunca, planeta i njihovih prirodnih satelita, planetama se dodeljuje plava, zelena ili crvena boja. Zatim se komete animiraju i prolaze kroz Suncev sistem.

U slucaju kolizije komete sa:

  * nekom od planeta, a u zavisnosti od trenutne boje planete desava se sledece:
    - PLAVA:  kometa postaje njen satelit,
    - ZELENA: ako planeta ima bar jedan satelit - kometa ga unistava, ako planeta nema satelite - biva unistena,
    - CRVENA: planeta biva unistena bez obzira na trenutni broj satelita.

  * u slucaju kolizije sa Suncem:
    - dok postoji bar jedna planeta: Sunce unistava komete, 
    - kada budu unistene sve planete pri sledecom kolizijom komete sa Suncem: Sunce eksplodira - kraj simulacije. 

Upravlja se sledecim tasterima:
  - 'G' : pokrece se animacija,
  - 'S' : objektima se dodeljuju boje,
  - '+' : ubrazava se rotacija planeta oko Sunca,
  - '-' : usporava se rotacija planeta oko Sunca,
  - 'P' : pauzira se simulacija.
  
