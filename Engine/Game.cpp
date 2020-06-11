/****************************************************************************************** 
 *	Chili DirectX Framework Version 16.07.20											  *	
 *	Game.cpp																			  *
 *	Copyright 2016 PlanetChili.net <http://www.planetchili.net>							  *
 *																						  *
 *	This file is part of The Chili DirectX Framework.									  *
 *																						  *
 *	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
 *	it under the terms of the GNU General Public License as published by				  *
 *	the Free Software Foundation, either version 3 of the License, or					  *
 *	(at your option) any later version.													  *
 *																						  *
 *	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
 *	GNU General Public License for more details.										  *
 *																						  *
 *	You should have received a copy of the GNU General Public License					  *
 *	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
 ******************************************************************************************/
#include "MainWindow.h"
#include "Game.h"
#include "SpriteCodex.h"

Game::Game( MainWindow& wnd )
	:
	wnd( wnd ),
	gfx( wnd ),
	rng( std::random_device()() ),
	snek( {2,2} )
{
	CreateBoard2();
	for( int i = 0; i < nPoison; i++ )
	{
		pBrd->SpawnContents( rng,snek,Board::CellContents::Poison );
	}
	for( int i = 0; i < nFood; i++ )
	{
		pBrd->SpawnContents( rng,snek,Board::CellContents::Food );
	}
	sndTitle.Play( 1.0f,1.0f );
}

void Game::Go()
{
	gfx.BeginFrame();	
	UpdateModel();
	ComposeFrame();
	gfx.EndFrame();
}

void Game::UpdateModel()
{
	const float dt = ft.Mark();
	
	if( gameIsStarted )
	{
		if( !gameIsOver )
		{
			if( wnd.kbd.KeyIsPressed( VK_UP ) )
			{
				delta_loc = { 0,-1 };
			}
			else if( wnd.kbd.KeyIsPressed( VK_DOWN ) )
			{
				delta_loc = { 0,1 };
			}
			else if( wnd.kbd.KeyIsPressed( VK_LEFT ) )
			{
				delta_loc = { -1,0 };
			}
			else if( wnd.kbd.KeyIsPressed( VK_RIGHT ) )
			{
				delta_loc = { 1,0 };
			}

			float snekModifiedMovePeriod = snekMovePeriod;
			if( wnd.kbd.KeyIsPressed( VK_CONTROL ) )
			{
				snekModifiedMovePeriod = std::min( snekMovePeriod,snekMovePeriodSpeedup );
			}

			snekMoveCounter += dt;
			if( snekMoveCounter >= snekModifiedMovePeriod )
			{
				snekMoveCounter -= snekModifiedMovePeriod;
				const Location next = snek.GetNextHeadLocation( delta_loc );
				const Board::CellContents contents = pBrd->GetContents( next );
				if( !pBrd->IsInsideBoard( next ) ||
					snek.IsInTileExceptEnd( next ) ||
					contents == Board::CellContents::Obstacle )
				{
					gameIsOver = true;
					sndFart.Play( rng,1.2f );
					sndMusic.StopAll();
				}
				else if( contents == Board::CellContents::Food )
				{
					snek.GrowAndMoveBy( delta_loc );
					pBrd->ConsumeContents( next );
					pBrd->SpawnContents( rng,snek,Board::CellContents::Obstacle );
					pBrd->SpawnContents( rng,snek,Board::CellContents::Food );
					sfxEat.Play( rng,0.8f );
				}
				else if( contents == Board::CellContents::Poison )
				{
					snek.MoveBy( delta_loc );
					pBrd->ConsumeContents( next );
					snekMovePeriod = std::max( snekMovePeriod * snekSpeedupFactor,snekMovePeriodMin );
					sndFart.Play( rng,0.6f );
				}
				else
				{
					snek.MoveBy( delta_loc );
					sfxSlither.Play( rng,0.08f );
				}
			}
		}
	}
	else
	{
		if( wnd.kbd.KeyIsPressed( VK_RETURN ) )
		{
			sndMusic.Play( 1.0f,0.6f );
			gameIsStarted = true;
		}
	}
}

void Game::CreateBoard2()
{
	std::ifstream in("Config.txt");
	std::string str;

	int width;
	int height;
	int nPoison;
	int nFood;
	int dimension;
	float speedupRate;

	while (!in.eof())
	{
		std::getline(in, str);

		if (str == "[Tile Size]")
		{
			std::getline(in, str);
			dimension = std::stoi(str);
		}
		else if (str == "[Board Size]")
		{
			std::getline(in, str);
			int whiteSpace = (int)str.find(" ");
			width = std::stoi(str.substr(0, whiteSpace));
			height = std::stoi(str.substr(whiteSpace + 1, str.size()));
		}
		else if (str == "[Speedup Rate]")
		{
			std::getline(in, str);
			speedupRate = std::stof(str);
		}
		else if (str == "[Poison Amount]")
		{
			std::getline(in, str);
			nPoison = std::stoi(str);
		}
		else if (str == "[Goal Amount]")
		{
			std::getline(in, str);
			nFood = std::stoi(str);
		}
	}
	pBrd = new Board(gfx, dimension, width, height);
	this->nPoison = nPoison;
	this->nFood = nFood;
	this->snekSpeedupFactor = 2 - speedupRate;
}

void Game::ComposeFrame()
{
	if( gameIsStarted )
	{
		snek.Draw( *pBrd );
		pBrd->DrawCells();
		if( gameIsOver )
		{
			SpriteCodex::DrawGameOver( 350,265,gfx );
		}
		pBrd->DrawBorder();
	}
	else
	{
		SpriteCodex::DrawTitle( 290,225,gfx );
	}
}
