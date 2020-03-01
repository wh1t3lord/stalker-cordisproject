/*
 * Copyright (C) 2017 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. AND ITS CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

WebInspector.WebSocketContentView = class WebSocketContentView extends WebInspector.ContentView
{
    constructor(resource)
    {
        console.assert(resource instanceof WebInspector.WebSocketResource, resource);

        super(resource);

        this._resource = resource;
        this._framesRendered = 0;
        this._lastRenderedReadyState = null;

        // COMPATIBILITY (iOS 10.3): `walltime` did not exist in 10.3 and earlier.
        this._showTimeColumn = NetworkAgent.hasEventParameter("webSocketWillSendHandshakeRequest", "walltime");

        this.element.classList.add("web-socket", "resource");

        let columns = {data: {}};
        columns.data.title = WebInspector.UIString("Data");
        columns.data.sortable = false;
        columns.data.width = "85%";

        if (this._showTimeColumn)
            columns.time = {title: WebInspector.UIString("Time"), sortable: true};

        this._dataGrid = new WebInspector.DataGrid(columns);
        this._dataGrid.variableHeightRows = true;
        this.addSubview(this._dataGrid);

        this._addRow(WebInspector.UIString("WebSocket Connection Established"), this._resource.walltime);

        this._dataGrid.updateLayout();
    }

    // Static

    static textForOpcode(opcode)
    {
        switch (opcode) {
        case WebInspector.WebSocketResource.OpCodes.ContinuationFrame:
            return WebInspector.UIString("Continuation Frame");
        case WebInspector.WebSocketResource.OpCodes.TextFrame:
            return WebInspector.UIString("Text Frame");
        case WebInspector.WebSocketResource.OpCodes.BinaryFrame:
            return WebInspector.UIString("Binary Frame");
        case WebInspector.WebSocketResource.OpCodes.ConnectionCloseFrame:
            return WebInspector.UIString("Connection Close Frame");
        case WebInspector.WebSocketResource.OpCodes.PingFrame:
            return WebInspector.UIString("Ping Frame");
        case WebInspector.WebSocketResource.OpCodes.PongFrame:
            return WebInspector.UIString("Pong Frame");
        }
    }

    // Public

    shown()
    {
        this._updateFrames();
        this._resource.addEventListener(WebInspector.WebSocketResource.Event.FrameAdded, this._updateFramesSoon, this);
        this._resource.addEventListener(WebInspector.WebSocketResource.Event.ReadyStateChanged, this._updateFramesSoon, this);
    }

    hidden()
    {
        this._resource.removeEventListener(WebInspector.WebSocketResource.Event.FrameAdded, this._updateFramesSoon, this);
        this._resource.removeEventListener(WebInspector.WebSocketResource.Event.ReadyStateChanged, this._updateFramesSoon, this);
    }

    // Private

    _updateFramesSoon()
    {
        this.onNextFrame._updateFrames();
    }

    _updateFrames()
    {
        let shouldScrollToBottom = this._dataGrid.isScrolledToLastRow();

        let framesLength = this._resource.frames.length;
        for (let index = this._framesRendered; index < framesLength; index++) {
            let frame = this._resource.frames[index];
            let {data, isOutgoing, opcode, walltime} = frame;
            this._addFrame(data, isOutgoing, opcode, walltime);
        }

        this._framesRendered = framesLength;

        if (this._lastRenderedReadyState !== this._resource.readyState) {
            if (this._resource.readyState === WebInspector.WebSocketResource.ReadyState.Closed)
                this._dataGrid.appendChild(new WebInspector.SpanningDataGridNode(WebInspector.UIString("Connection Closed")));

            this._lastRenderedReadyState = this._resource.readyState;
        }

        if (shouldScrollToBottom)
            this._dataGrid.onNextFrame.scrollToLastRow();
    }

    _addFrame(data, isOutgoing, opcode, time)
    {
        let nodeText;
        let isText = opcode === WebInspector.WebSocketResource.OpCodes.TextFrame;
        if (isText)
            nodeText = data;
        else
            nodeText = WebInspector.WebSocketContentView.textForOpcode(opcode);

        this._addRow(nodeText, time, {isOutgoing, isText});
    }

    _addRow(data, time, attributes = {})
    {
        let node;
        if (this._showTimeColumn)
            node = new WebInspector.WebSocketDataGridNode(Object.shallowMerge({data, time}, attributes));
        else
            node = new WebInspector.WebSocketDataGridNode(Object.shallowMerge({data}, attributes));

        this._dataGrid.appendChild(node);

        if (attributes.isText)
            node.element.classList.add("text-frame");
        else
            node.element.classList.add("non-text-frame");

        if (attributes.isOutgoing)
            node.element.classList.add("outgoing");
        else
            node.element.classList.add("incoming");
    }
};
