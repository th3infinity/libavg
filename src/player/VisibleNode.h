//
//  libavg - Media Playback Engine. 
//  Copyright (C) 2003-2008 Ulrich von Zadow
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//  Current versions can be found at www.libavg.de
//

#ifndef _VisibleNode_H_
#define _VisibleNode_H_

#include "Node.h"
#include "Event.h"

#include "../api.h"

#include "../base/Rect.h"
#include "../graphics/Pixel32.h"

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

// Python docs say python.h should be included before any standard headers (!)
#include "WrapPython.h" 

#include <string>
#include <map>

namespace avg {

class Scene;
class VisibleNode;
class DivNode;
class SceneNode;
class AVGNode;
class DisplayEngine;
class SDLDisplayEngine;
class AudioEngine;
class Image;
class VertexArray;

typedef boost::shared_ptr<VisibleNode> VisibleNodePtr;
typedef boost::weak_ptr<VisibleNode> VisibleNodeWeakPtr;
typedef boost::shared_ptr<DivNode> DivNodePtr;
typedef boost::weak_ptr<DivNode> DivNodeWeakPtr;
typedef boost::shared_ptr<SceneNode> SceneNodePtr;
typedef boost::weak_ptr<SceneNode> SceneNodeWeakPtr;
typedef boost::shared_ptr<AVGNode> AVGNodePtr;
typedef boost::weak_ptr<AVGNode> AVGNodeWeakPtr;
typedef boost::shared_ptr<Image> ImagePtr;
typedef boost::shared_ptr<VertexArray> VertexArrayPtr;

class AVG_API VisibleNode: public Node
{
    public:
        enum NodeState {NS_UNCONNECTED, NS_CONNECTED, NS_CANRENDER};
        
        static NodeDefinition createDefinition();
        
        virtual ~VisibleNode();
        virtual void setArgs(const ArgList& Args);
        virtual void setParent(DivNodeWeakPtr pParent, NodeState parentState,
                Scene * pScene);
        virtual void setRenderingEngines(DisplayEngine * pDisplayEngine, 
                AudioEngine * pAudioEngine);
        virtual void connect(Scene * pScene);
        virtual void disconnect(bool bKill);
        virtual void checkReload() {};

        void setID(const std::string& ID);

        double getOpacity() const;
        void setOpacity(double opacity);
        
        bool getActive() const;
        void setActive(bool bActive);
        
        bool getSensitive() const;
        void setSensitive(bool bSensitive);

        DivNodePtr getDivParent() const;
        std::vector<VisibleNodeWeakPtr> getParentChain() const;
        void unlink(bool bKill=false);

        void setMouseEventCapture();
        void releaseMouseEventCapture();
        void setEventCapture(int cursorID);
        void releaseEventCapture(int cursorID);
        void setEventHandler(Event::Type Type, int Sources, PyObject * pFunc);

        DPoint getRelPos(const DPoint& AbsPos) const;
        DPoint getAbsPos(const DPoint& RelPos) const;
        virtual DPoint toLocal(const DPoint& pos) const;
        virtual DPoint toGlobal(const DPoint& pos) const;
        virtual VisibleNodePtr getElementByPos(const DPoint & pos);

        virtual void preRender();
        virtual void maybeRender(const DRect& Rect) {};
        virtual void render(const DRect& Rect) {};
        virtual void renderOutlines(VertexArrayPtr pVA, Pixel32 color) {};

        double getEffectiveOpacity();
        virtual std::string dump(int indent = 0);
        
        NodeState getState() const;
        Scene * getScene() const;

        virtual bool handleEvent(EventPtr pEvent); 

    protected:
        VisibleNode();
        VisibleNodePtr getVThis() const;

        void addEventHandlers(Event::Type EventType, const std::string& Code);
        void addEventHandler(Event::Type EventType, Event::Source Source, 
                const std::string& Code);
        bool reactsToMouseEvents();
            
        SDLDisplayEngine * getDisplayEngine() const;
        AudioEngine * getAudioEngine() const;
        void setState(NodeState State);
        void initFilename(std::string& sFilename);
        void checkReload(const std::string& sHRef, const ImagePtr& pImage);

    private:
        PyObject * findPythonFunc(const std::string& Code);
        bool callPython(PyObject * pFunc, avg::EventPtr pEvent);

        struct EventHandlerID {
            EventHandlerID(Event::Type EventType, Event::Source Source);

            bool operator < (const EventHandlerID& other) const;

            Event::Type m_Type;
            Event::Source m_Source;
        };
        typedef std::map<EventHandlerID, PyObject *> EventHandlerMap;
        EventHandlerMap m_EventHandlerMap;

        Scene * m_pScene;
        SDLDisplayEngine * m_pDisplayEngine;
        AudioEngine * m_pAudioEngine;

        double m_Opacity;
        NodeState m_State;
        const NodeDefinition* m_pDefinition;

        bool m_bActive;
        bool m_bSensitive;
        double m_EffectiveOpacity;
};

}

#endif //_Node_H_