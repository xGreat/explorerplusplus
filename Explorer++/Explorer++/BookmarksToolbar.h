// Copyright (C) Explorer++ Project
// SPDX-License-Identifier: GPL-3.0-only
// See LICENSE in the top level directory

#pragma once

#include "BookmarkTree.h"
#include "CoreInterface.h"
#include "Navigation.h"
#include "ResourceHelper.h"
#include "../Helper/DpiCompatibility.h"
#include "../Helper/WindowSubclassWrapper.h"
#include <boost/signals2.hpp>
#include <wil/resource.h>

class CBookmarksToolbarDropHandler : public IDropTarget
{
public:

	CBookmarksToolbarDropHandler(HWND hToolbar, BookmarkTree *bookmarkTree);
	~CBookmarksToolbarDropHandler();

	/* IUnknown methods. */
	HRESULT __stdcall	QueryInterface(REFIID iid,void **ppvObject);
	ULONG __stdcall		AddRef(void);
	ULONG __stdcall		Release(void);

private:

	CBookmarksToolbarDropHandler & operator = (const CBookmarksToolbarDropHandler &btdh);

	/* IDropTarget methods. */
	HRESULT __stdcall	DragEnter(IDataObject *pDataObject,DWORD grfKeyState,POINTL pt,DWORD *pdwEffect);
	HRESULT __stdcall	DragOver(DWORD grfKeyState,POINTL pt,DWORD *pdwEffect);
	HRESULT __stdcall	DragLeave(void);
	HRESULT __stdcall	Drop(IDataObject *pDataObject,DWORD grfKeyState,POINTL pt,DWORD *pdwEffect);

	int					GetToolbarPositionIndex(const POINTL &pt,bool &bAfter);
	void				RemoveInsertionMark();

	ULONG m_ulRefCount;

	HWND m_hToolbar;
	BookmarkTree *m_bookmarkTree;

	IDragSourceHelper *m_pDragSourceHelper;
	IDropTargetHelper *m_pDropTargetHelper;
	bool m_bAcceptData;
};

class CBookmarksToolbar
{
public:

	CBookmarksToolbar(HWND hToolbar, HINSTANCE instance, IExplorerplusplus *pexpp,
		Navigation *navigation, BookmarkTree *bookmarkTree, UINT uIDStart, UINT uIDEnd);
	~CBookmarksToolbar();

private:

	CBookmarksToolbar & operator = (const CBookmarksToolbar &bt);

	static const UINT_PTR SUBCLASS_ID = 0;
	static const UINT_PTR PARENT_SUBCLASS_ID = 0;

	static LRESULT CALLBACK	BookmarksToolbarProcStub(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);
	LRESULT CALLBACK	BookmarksToolbarProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);

	static LRESULT CALLBACK	BookmarksToolbarParentProcStub(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);
	LRESULT CALLBACK	BookmarksToolbarParentProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);

	void	InitializeToolbar();

	void	InsertBookmarkItems();
	void	InsertBookmarkItem(BookmarkItem *bookmarkItem, int position);

	void	RemoveBookmarkItem(const BookmarkItem *bookmarkItem);

	void	OpenBookmarkItemInNewTab(const BookmarkItem *bookmarkItem);

	bool	OnCommand(WPARAM wParam, LPARAM lParam);
	bool	OnButtonClick(int command);
	BOOL	OnRightClick(const NMMOUSE *nmm);
	void	OnRightClickMenuItemSelected(int menuItemId, BookmarkItem *bookmarkItem);
	void	ShowBookmarkFolderMenu(const BookmarkItem *bookmarkItem, int command, int index);
	void	OnBookmarkMenuItemClicked(const BookmarkItem *bookmarkItem);
	void	OnNewBookmark();
	bool	OnGetInfoTip(NMTBGETINFOTIP *infoTip);

	void	OnToolbarContextMenuPreShow(HMENU menu, HWND sourceWindow);

	std::optional<int>	GetBookmarkItemIndex(const BookmarkItem *bookmarkItem) const;

	BookmarkItem	*GetBookmarkItemFromToolbarIndex(int index);

	void	OnBookmarkItemAdded(BookmarkItem &bookmarkItem, size_t index);
	void	OnBookmarkItemUpdated(BookmarkItem &bookmarkItem, BookmarkItem::PropertyType propertyType);
	void	OnBookmarkItemPreRemoval(BookmarkItem &bookmarkItem);

	HWND m_hToolbar;
	DpiCompatibility m_dpiCompat;
	wil::unique_himagelist m_imageList;
	IconImageListMapping m_imageListMappings;

	HINSTANCE m_instance;

	IExplorerplusplus *m_pexpp;
	Navigation *m_navigation;

	BookmarkTree *m_bookmarkTree;

	UINT m_uIDStart;
	UINT m_uIDEnd;
	UINT m_uIDCounter;

	CBookmarksToolbarDropHandler *m_pbtdh;

	std::vector<WindowSubclassWrapper> m_windowSubclasses;
	std::vector<boost::signals2::scoped_connection> m_connections;
};