This XML parser and Tilemap loader is designed for Simple Fast multimedia Library using c++.
This will load tilemaps as texture in an sfml window
All Author credit ges to Caplitero
I wrote down his code while watching his youtube video, and it seems to work well, though I personally have not tried editing any layers in the tile maps


Here is an example use case
__________________________________________________________________________________________________________________________________________________________

int main()
{

      Dinzai::SFMLMap* Map;
      
      Map = new Dinzai::SFMLMap("maps/", "test.tmx");

      sf::RenderWindow window(sf::VideoMode(1366, 768).getDesktopMode(), "TEST");


      while (window.isOpen())
      {

            sf::Event event;
            while (window.pollEvent(event))
            {

                  window.close();
          
            }

                sf::RenderStates states;
                
                window.clear();

                Map->draw(window, states);
              
                window.display();

      }



}

This is a very simplified version, some functinality may be missing in the example 

the important thing is you have a window, and states for the map to reference as arguments 





