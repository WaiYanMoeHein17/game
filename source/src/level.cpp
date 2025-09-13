#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <string>
#include <cmath> 
#include <algorithm>
#include <sstream>
#include <map>
#include <vector>
#include "tile.h"
#include "level.h"
#include "graphics.h"
#include "globals.h"
#include "rectangle.h"
#include "tinyxml2.h"

using namespace tinyxml2;
using namespace std;

Level::Level() {

}

Level::Level(string mapName, Vector2D spawnPoint, Graphics &graphics) 
    : _mapName(mapName), 
    //_spawnPoint(spawnPoint), 
    _size(_size.zero()),
    _tileSize(_tileSize.zero())
    //_backgroundTexture(nullptr)
    {
    loadMap3(mapName, graphics);
}

Level::~Level() {
    if (_backgroundTexture) {
        SDL_DestroyTexture(_backgroundTexture);
    }
}

void Level::loadMap3(string mapName, Graphics &graphics) {
    cout << "Loading TMX map: " << mapName << endl;
    
    XMLDocument mapFile;
    stringstream ss; 
    ss << "../content/maps/" << mapName << ".tmx";
    
    if (mapFile.LoadFile(ss.str().c_str()) != XML_SUCCESS) {
        cerr << "Failed to load map file: " << ss.str() << endl;
        return;
    }
    
    XMLElement* mapElement = mapFile.FirstChildElement("map");
    if (!mapElement) {
        cerr << "No map element found!" << endl;
        return;
    }
    
    // Get map dimensions
    int mapWidth, mapHeight, tileWidth, tileHeight;
    mapElement->QueryIntAttribute("width", &mapWidth);
    mapElement->QueryIntAttribute("height", &mapHeight);
    mapElement->QueryIntAttribute("tilewidth", &tileWidth);
    mapElement->QueryIntAttribute("tileheight", &tileHeight);
    
    _size = Vector2D(mapWidth * tileWidth, mapHeight * tileHeight);
    _tileSize = Vector2D(tileWidth, tileHeight);
    
    cout << "Map size: " << mapWidth << "x" << mapHeight << " tiles" << endl;
    cout << "Tile size: " << tileWidth << "x" << tileHeight << " pixels" << endl;
    
    // Load tileset
    string tilesetPath = "../content/background2/Tilesheet/mapPack_tilesheet.png";
    SDL_Surface* surface = graphics.loadSurface(tilesetPath);
    if (surface) {
        SDL_Texture* texture = SDL_CreateTextureFromSurface(graphics.getRenderer(), surface);
        SDL_DestroySurface(surface);
        
        if (texture) {
            _tilesets.push_back(Tileset(texture, 1));
            cout << "Loaded tileset directly: " << tilesetPath << endl;
        }
    }
    
    // Load tile layers
    XMLElement* layerElement = mapElement->FirstChildElement("layer");
    while (layerElement) {
        XMLElement* dataElement = layerElement->FirstChildElement("data");
        if (dataElement) {
            const char* encoding = dataElement->Attribute("encoding");
            if (encoding && string(encoding) == "csv") {
                string csvData = dataElement->GetText();
                parseCSVTileData(csvData, mapWidth, mapHeight, tileWidth, tileHeight);
            }
        }
        layerElement = layerElement->NextSiblingElement("layer");
    }

    /*XMLElement* objectGroupElement = mapElement->FirstChildElement("objectgroup");
    if (objectGroupElement) {
        while (objectGroupElement) {
            stringstream ss; 
            const char* name = objectGroupElement->Attribute("name");
            if (name && string(name) == "collisions") {
                XMLElement* objectElement = objectGroupElement->FirstChildElement("object");
                if (objectElement) {
                    while (objectElement) {
                        float x, y, width, height;
                        x = objectElement->FloatAttribute("x");
                        y = objectElement->FloatAttribute("y");
                        width = objectElement->FloatAttribute("width");
                        height = objectElement->FloatAttribute("height");
                        _collisionRects.push_back(Rectangle(ceil(x), ceil(y), ceil(width), ceil(height)));
                        objectElement = objectElement->NextSiblingElement("object");
                    }  
                    cout << "Parsed " << _collisionRects.size() << " collision rectangles." << endl;
                }
            }
            objectGroupElement = objectGroupElement->NextSiblingElement("objectgroup");
        }
    }*/ 
    
    //NEW: Load collision object groups
    XMLElement* objectGroupElement = mapElement->FirstChildElement("objectgroup");
    while (objectGroupElement) {
        const char* layerName = objectGroupElement->Attribute("name");
        if (layerName && string(layerName) == "collisions") {
            parseCollisionObjects(objectGroupElement);
        }
        objectGroupElement = objectGroupElement->NextSiblingElement("objectgroup");
    }
    
    cout << "Map loaded successfully! Created " << _tileList.size() << " tiles and " << _collisionRects.size() << " collision objects." << endl;
}

void Level::loadExternalTileset(const string& tsxPath, int firstGid, Graphics& graphics) {
    XMLDocument tsxFile;
    stringstream ss;
    ss << "../content" << tsxPath;  // TSX path is relative to content folder
    
    if (tsxFile.LoadFile(ss.str().c_str()) != XML_SUCCESS) {
        cerr << "Failed to load external tileset: " << ss.str() << endl;
        return;
    }
    
    XMLElement* tilesetElement = tsxFile.FirstChildElement("tileset");
    if (!tilesetElement) return;
    
    XMLElement* imageElement = tilesetElement->FirstChildElement("image");
    if (imageElement) {
        const char* imagePath = imageElement->Attribute("source");
        stringstream imageSS;
        imageSS << "../content/background2/Tilesheet/" << imagePath;  // Adjust path as needed
        
        SDL_Surface* surface = graphics.loadSurface(imageSS.str());
        if (surface) {
            SDL_Texture* texture = SDL_CreateTextureFromSurface(graphics.getRenderer(), surface);
            SDL_DestroySurface(surface);
            
            if (texture) {
                _tilesets.push_back(Tileset(texture, firstGid));
                cout << "Loaded external tileset: " << imagePath << endl;
            }
        } else {
            cerr << "Failed to load tileset image: " << imageSS.str() << endl;
        }
    }
}


void Level::loadMap2(string mapName, Graphics &graphics) {
    // parse the .tmx file and load tilesets and tiles
    XMLDocument mapFile;
    stringstream ss; 
    ss << "..//content//maps//" << mapName << ".tmx";
    mapFile.LoadFile(ss.str().c_str());
    if (mapFile.Error()) {
        cerr << "Failed to load map file: " << ss.str() << " Error: " << mapFile.ErrorIDToName(mapFile.ErrorID()) << endl;
        return;
    }
    XMLElement* mapElement = mapFile.FirstChildElement("map");
    if (mapElement == nullptr) {
        cerr << "No <map> element found in map file: " << ss.str() << endl;
        return;
    }
    // get the width, height, tilewidth, tileheight attributes
    int mapWidth = mapElement->QueryIntAttribute("width", &mapWidth) == XML_SUCCESS ? mapWidth : 0;
    int mapHeight = mapElement->QueryIntAttribute("height", &mapHeight) == XML_SUCCESS ? mapHeight : 0;
    int tileWidth = mapElement->QueryIntAttribute("tilewidth", &tileWidth) == XML_SUCCESS ? tileWidth : 0;
    int tileHeight = mapElement->QueryIntAttribute("tileheight", &tileHeight) == XML_SUCCESS ? tileHeight : 0;
    
    // load tilesets
    XMLElement* tilesetElement = mapElement->FirstChildElement("tileset");
    if (tilesetElement != nullptr) {
        while (tilesetElement) {
            int firstGid;
            const char* source = tilesetElement->FirstChildElement("image")->Attribute("source");
            char* path; 
            stringstream ss;
            ss << "..//content//maps//" << source;
            tilesetElement->QueryIntAttribute("firstgid", &firstGid);
            SDL_Texture* texture = SDL_CreateTextureFromSurface(graphics.getRenderer(), graphics.loadSurface(ss.str()));
            _tilesets.push_back(Tileset(texture, firstGid));
            //cout << "Loading tileset image from: " << ss.str() << endl;
            tilesetElement = tilesetElement->NextSiblingElement("tileset");
        }
    }

    // load layers and tiles
    XMLElement* layerElement = mapElement->FirstChildElement("layer");
    if (layerElement != nullptr) {
        while (layerElement) {
            XMLElement* dataElement = layerElement->FirstChildElement("data");
            if (dataElement != nullptr) {
                while (dataElement) {
                    XMLElement* tileElement = dataElement->FirstChildElement("tile");
                    if (tileElement != nullptr){
                        int tileCtr = 0;
                        while (tileElement) {
                            // get the gid attribute
                            // if gid is 0, no tile to draw
                            if (tileElement->IntAttribute("gid") == 0) {
                                tileCtr++; 
                                if (tileElement->NextSiblingElement("tile")) {
                                    tileElement = tileElement->NextSiblingElement("tile");
                                    continue;
                                } else {
                                    break; 
                                }
                            }
                            // get the tileset for this gid
                            int gid = tileElement->IntAttribute("gid");
                            Tileset tls; 
                            for (int i = 0; i < _tilesets.size(); i++) {
                                if (_tilesets[i].FirstGid <= gid) {
                                    tls = _tilesets.at(i);
                                    break; 
                                }
                            }
                            if (tls.FirstGid == -1) {
                                cerr << "No tileset found for gid: " << gid << endl;
                                tileCtr++;
                                if (tileElement->NextSiblingElement("tile")) {
                                    tileElement = tileElement->NextSiblingElement("tile");
                                    continue;
                                } else {
                                    break; 
                                }
                            }

                            // calculate the position of the tile in the tileset
                            int xx = 0;
                            int yy = 0; 
                            xx = tileCtr % mapWidth;
                            xx = xx * tileWidth; 
                            yy = tileCtr % mapHeight;
                            yy = yy * tileHeight;

                            tileElement = tileElement->NextSiblingElement("tile");
                        }
                    }

                }
            }
            layerElement = layerElement->NextSiblingElement("layer");
            
        }
    }

}

void Level::loadMap1(string mapName, Graphics &graphics) {
    cout << "Attempting to load background..." << endl;

    SDL_Surface* surface = graphics.loadSurface("../content/background2/PNG/mapTile_001.png");
    if (surface) {

        int surfaceWidth = surface->w;
        int surfaceHeight = surface->h;

        _backgroundTexture = SDL_CreateTextureFromSurface(graphics.getRenderer(), surface);
        SDL_DestroySurface(surface); // Free the surface after creating the texture

        if (_backgroundTexture) {
            cout << "Texture created successfully!" << endl;
            _size = Vector2D(static_cast<float>(surfaceWidth), static_cast<float>(surfaceHeight));
        } else {
            cout << "Failed to create texture: " << SDL_GetError() << endl;
        }
    } else {
        cout << "Failed to load texture!" << endl;
    }
}

void Level::parseCollisionObjects(XMLElement* objectGroupElement) {
    cout << "parseCollisionObjects called!" << endl;
    if (!objectGroupElement) {
        cerr << "No object group element provided!" << endl;
        return;
    }
    XMLElement* objectElement = objectGroupElement->FirstChildElement("object");
    int objectCount = 0;
    while (objectElement) {
        float x, y, width, height;
        
        if (objectElement->QueryFloatAttribute("x", &x) != XML_SUCCESS) {
            cout << "Warning: No x attribute for object " << objectCount << endl;
            x = 0.0f;
        }
        if (objectElement->QueryFloatAttribute("y", &y) != XML_SUCCESS) {
            cout << "Warning: No y attribute for object " << objectCount << endl;
            y = 0.0f;
        }
        if (objectElement->QueryFloatAttribute("width", &width) != XML_SUCCESS) {
            cout << "Warning: No width attribute for object " << objectCount << endl;
            width = 32.0f;
        }
        if (objectElement->QueryFloatAttribute("height", &height) != XML_SUCCESS) {
            cout << "Warning: No height attribute for object " << objectCount << endl;
            height = 32.0f;
        }
        _collisionRects.push_back(Rectangle(ceil(x), ceil(y), ceil(width), ceil(height)));
        objectElement = objectElement->NextSiblingElement("object");
        objectCount++;
    }
    cout << "Parsed " << _collisionRects.size() << " collision rectangles." << endl;
}

/*void Level::loadMap(string mapName, Graphics &graphics) {
    cout << "Attempting to load: " << mapName << endl;
    cout << "Looking for file at: ../content/background/Samples/colored_castle.png" << endl;

    SDL_Surface* backgroundSurface = graphics.loadImage("..//content//background//Samples//colored_castle.png");
    if (backgroundSurface){
        // Get size from surface before creating texture
        int surfaceWidth = backgroundSurface->w;
        int surfaceHeight = backgroundSurface->h;
        
        _backgroundTexture = SDL_CreateTextureFromSurface(graphics.getRenderer(), backgroundSurface);
        SDL_DestroySurface(backgroundSurface);
        
        if (_backgroundTexture) {
            _size = Vector2D(static_cast<float>(surfaceWidth), static_cast<float>(surfaceHeight));
            cout << "Loaded background texture with size: " << surfaceWidth << "x" << surfaceHeight << endl;
        } else {
            cerr << "Failed to create texture from surface: " << SDL_GetError() << endl;
            _size = Vector2D(800, 600);
        }
    } else {
        cerr << "Failed to load background image: " << SDL_GetError() << endl;
    }
}*/ 

void Level::update(int elapsedTime) {
    // Update level state if needed
}

vector<Rectangle> Level::checkTileCollisions(const Rectangle &other) {
    vector<Rectangle> collidingRects; 
    for (auto& rect : _collisionRects) {
        if (other.collidesWith(rect)) {
            collidingRects.push_back(rect);
        }
    }
    return collidingRects;
}

void Level::draw2(Graphics &graphics) {
    for (auto& tile : _tileList) {
        tile.draw(graphics);
    }

    for (auto& rect : _collisionRects) {
        //rect.debugDraw(graphics);
    }

    if (_tileList.empty()) {
        // Draw red screen so you know this code path is running
        SDL_SetRenderDrawColor(graphics.getRenderer(), 255, 0, 0, 255);
        SDL_RenderClear(graphics.getRenderer());
        cout << "No tiles to draw!" << endl;
    }
}

void Level::draw1(Graphics &graphics) {
    if (_backgroundTexture) {
        // Simple full-screen draw first
        SDL_FRect sourceRect = {0, 0, _size.x, _size.y}; // Full source image
        SDL_FRect destRect = {0, 0, (float)globals::SCREEN_WIDTH, (float)globals::SCREEN_HEIGHT};
        //SDL_FRect destRect; 
        //destRect.w = 64; 
        //destRect.h = 64;
        /*for (int x = 0; x < _size.x / 64 ; x++) {
            for (int y = 0; y < _size.y / 64; y++) {
                destRect.x = x * 64 * globals::SPRITE_SCALE;
                destRect.y = y * 64 * globals::SPRITE_SCALE;
                graphics.blitSurface(_backgroundTexture, &sourceRect, &destRect);
            }
        }*/ 
        graphics.blitSurface(_backgroundTexture, &sourceRect, &destRect);
        //cout << "Drawing background texture" << endl; // Remove this after testing
    } else {
        //cout << "No background texture to draw!" << endl;
        // Draw red screen so you know this code path is running
        SDL_SetRenderDrawColor(graphics.getRenderer(), 255, 0, 0, 255);
        SDL_RenderClear(graphics.getRenderer());
    }
    /*
    for (int x = 0; x < _size.x; x += tileWidth) {
        for (int y = 0; y < _size.y; y += tileHeight) {
            SDL_FRect destRect = {static_cast<float>(x), static_cast<float>(y), static_cast<float>(tileWidth), static_cast<float>(tileHeight)};
            graphics.blitSurface(_backgroundTexture, &sourceRect, &destRect);
        }
    }
    */
}

void Level::parseCSVTileData(const string &data, int width, int height, int tileWidth, int tileHeight) {
    // Decode base64 and decompress if needed, then parse CSV tile data
    // This is a placeholder implementation
    cout << "Parsing tile data..." << endl;
    stringstream ss(data);
    string token; 
    vector<int> tileGids; 

    while (getline(ss, token, ',')) {
        token.erase(remove_if(token.begin(), token.end(), ::isspace), token.end());
        if (!token.empty()) {
            tileGids.push_back(stoi(token));
        }
    }

    for (int i = 0; i < tileGids.size(); i++) {
        int gid = tileGids[i];
        if (gid == 0) {
            continue; // No tile to draw
        }
        int xx = (i % width) * tileWidth;
        int yy = (i / width) * tileHeight;
        // Find the correct tileset for this gid
        Tileset* tls;
        for (auto& ts : _tilesets) {
            if (ts.FirstGid <= gid) {
                tls = &ts; 
            } else {
                break; 
            }
        }
        if (tls) { 
            int localId = gid - tls->FirstGid;
            int tilesPerRow = 32; // Adjust based on your tileset
            int sourceX = (localId % tilesPerRow) * tileWidth;
            int sourceY = (localId / tilesPerRow) * tileHeight;
            
            //cout << "Creating tile at (" << xx << ", " << yy << ") from gid " << gid << " (localId " << localId << ") using tileset with firstGid " << tls->FirstGid << endl;
            // Create tile
            Tile tile(tls->texture, 
                     Vector2D(tileWidth, tileHeight),
                     Vector2D(sourceX, sourceY),
                     Vector2D(xx, yy));
            
            _tileList.push_back(tile);
        }
    }

}



