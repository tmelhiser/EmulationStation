#include "BasicGameListView.h"
#include "../ViewController.h"
#include "../../Renderer.h"
#include "../../Window.h"
#include "../../ThemeData.h"
#include "../../SystemData.h"

BasicGameListView::BasicGameListView(Window* window, FileData* root)
	: ISimpleGameListView(window, root), mList(window)
{
	mList.setSize(mSize.x(), mSize.y() * 0.8f);
	mList.setPosition(0, mSize.y() * 0.2f);
	addChild(&mList);

	populateList(root->getChildren());
}

void BasicGameListView::onThemeChanged(const std::shared_ptr<ThemeData>& theme)
{
	ISimpleGameListView::onThemeChanged(theme);
	using namespace ThemeFlags;
	mList.applyTheme(theme, getName(), "gamelist", ALL);
}

void BasicGameListView::onFileChanged(FileData* file, FileChangeType change)
{
	if(change == FILE_METADATA_CHANGED)
	{
		// might switch to a detailed view
		mWindow->getViewController()->reloadGameListView(this);
		return;
	}

	ISimpleGameListView::onFileChanged(file, change);
}

void BasicGameListView::populateList(const std::vector<FileData*>& files)
{
	mList.clear();

	mHeaderText.setText(files.at(0)->getSystem()->getFullName());

	for(auto it = files.begin(); it != files.end(); it++)
	{
		mList.add((*it)->getName(), *it, ((*it)->getType() == FOLDER));
	}
}

FileData* BasicGameListView::getCursor()
{
	return mList.getSelected();
}

void BasicGameListView::setCursor(FileData* cursor)
{
	if(!mList.setCursor(cursor))
	{
		populateList(cursor->getParent()->getChildren());
		mList.setCursor(cursor);
	}
}

void BasicGameListView::launch(FileData* game)
{
	mWindow->getViewController()->launch(game);
}

std::vector<HelpPrompt> BasicGameListView::getHelpPrompts()
{
	std::vector<HelpPrompt> prompts;
	prompts.push_back(HelpPrompt("left/right", "change systems"));
	prompts.push_back(HelpPrompt("up/down", "scroll"));
	prompts.push_back(HelpPrompt("a", "play"));
	prompts.push_back(HelpPrompt("b", "back"));
	return prompts;
}