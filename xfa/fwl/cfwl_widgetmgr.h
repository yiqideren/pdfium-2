// Copyright 2014 PDFium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// Original code copyright 2014 Foxit Software Inc. http://www.foxitsoftware.com

#ifndef XFA_FWL_CFWL_WIDGETMGR_H_
#define XFA_FWL_CFWL_WIDGETMGR_H_

#include <map>
#include <memory>
#include <vector>

#include "core/fxcrt/fx_system.h"
#include "xfa/fwl/ifwl_widgetmgrdelegate.h"
#include "xfa/fxgraphics/cfx_graphics.h"

#define FWL_WGTMGR_DisableForm 0x00000002

class CFWL_Message;
class CXFA_FFApp;
class CXFA_FWLAdapterWidgetMgr;
class CFX_Graphics;
class CFX_Matrix;
class CFWL_Widget;

class CFWL_WidgetMgr : public CFWL_WidgetMgrDelegate {
 public:
  explicit CFWL_WidgetMgr(CXFA_FFApp* pAdapterNative);
  ~CFWL_WidgetMgr();

  // CFWL_WidgetMgrDelegate
  void OnSetCapability(uint32_t dwCapability) override;
  void OnProcessMessageToForm(CFWL_Message* pMessage) override;
  void OnDrawWidget(CFWL_Widget* pWidget,
                    CFX_Graphics* pGraphics,
                    const CFX_Matrix* pMatrix) override;

  CFWL_Widget* GetParentWidget(CFWL_Widget* pWidget) const;
  CFWL_Widget* GetOwnerWidget(CFWL_Widget* pWidget) const;
  CFWL_Widget* GetNextSiblingWidget(CFWL_Widget* pWidget) const;
  CFWL_Widget* GetFirstChildWidget(CFWL_Widget* pWidget) const;
  CFWL_Widget* GetSystemFormWidget(CFWL_Widget* pWidget) const;

  void RepaintWidget(CFWL_Widget* pWidget, const CFX_RectF& pRect);

  void InsertWidget(CFWL_Widget* pParent, CFWL_Widget* pChild);
  void RemoveWidget(CFWL_Widget* pWidget);
  void SetOwner(CFWL_Widget* pOwner, CFWL_Widget* pOwned);
  void SetParent(CFWL_Widget* pParent, CFWL_Widget* pChild);

  CFWL_Widget* GetWidgetAtPoint(CFWL_Widget* pParent,
                                const CFX_PointF& point) const;
  CFWL_Widget* NextTab(CFWL_Widget* parent, CFWL_Widget* focus, bool& bFind);

  std::vector<CFWL_Widget*> GetSameGroupRadioButton(
      CFWL_Widget* pRadioButton) const;

  CFWL_Widget* GetDefaultButton(CFWL_Widget* pParent) const;
  void AddRedrawCounts(CFWL_Widget* pWidget);

  bool IsFormDisabled() const {
    return !!(m_dwCapability & FWL_WGTMGR_DisableForm);
  }

  void GetAdapterPopupPos(CFWL_Widget* pWidget,
                          FX_FLOAT fMinHeight,
                          FX_FLOAT fMaxHeight,
                          const CFX_RectF& rtAnchor,
                          CFX_RectF& rtPopup) const;

 private:
  class Item {
   public:
    Item();
    explicit Item(CFWL_Widget* widget);
    ~Item();

    Item* pParent;
    Item* pOwner;
    Item* pChild;
    Item* pPrevious;
    Item* pNext;
    CFWL_Widget* const pWidget;
    std::unique_ptr<CFX_Graphics> pOffscreen;
    int32_t iRedrawCounter;
#if (_FX_OS_ == _FX_WIN32_DESKTOP_) || (_FX_OS_ == _FX_WIN64_)
    bool bOutsideChanged;
#endif
  };

  CFWL_Widget* GetFirstSiblingWidget(CFWL_Widget* pWidget) const;
  CFWL_Widget* GetPriorSiblingWidget(CFWL_Widget* pWidget) const;
  CFWL_Widget* GetLastChildWidget(CFWL_Widget* pWidget) const;
  Item* GetWidgetMgrItem(CFWL_Widget* pWidget) const;

  void AppendWidget(CFWL_Widget* pWidget);

  int32_t CountRadioButtonGroup(CFWL_Widget* pFirst) const;
  CFWL_Widget* GetRadioButtonGroupHeader(CFWL_Widget* pRadioButton) const;

  void ResetRedrawCounts(CFWL_Widget* pWidget);

  void DrawChild(CFWL_Widget* pParent,
                 const CFX_RectF& rtClip,
                 CFX_Graphics* pGraphics,
                 const CFX_Matrix* pMatrix);
  CFX_Graphics* DrawWidgetBefore(CFWL_Widget* pWidget,
                                 CFX_Graphics* pGraphics,
                                 const CFX_Matrix* pMatrix);
  bool IsNeedRepaint(CFWL_Widget* pWidget,
                     CFX_Matrix* pMatrix,
                     const CFX_RectF& rtDirty);

  bool IsAbleNative(CFWL_Widget* pWidget) const;

  uint32_t m_dwCapability;
  std::map<CFWL_Widget*, std::unique_ptr<Item>> m_mapWidgetItem;
  CXFA_FWLAdapterWidgetMgr* const m_pAdapter;
#if (_FX_OS_ == _FX_WIN32_DESKTOP_) || (_FX_OS_ == _FX_WIN64_)
  CFX_RectF m_rtScreen;
#endif
};

#endif  // XFA_FWL_CFWL_WIDGETMGR_H_
