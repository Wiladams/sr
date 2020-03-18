#pragma once

#include "p5.hpp"

/*
* <dt> base
* <dd> The base color is the color that is used as the "dominant" color 
*      for graphical objects. For example, a button's text is drawn on top 
*      of this color when the button is "up".  
* <dt> highlight
* <dd> A lighter rendition of the base color used to create a highlight in 
*      pseudo 3D effects.
* <dt> shadow
* <dd> A darker rendition of the base color used to create a shadow in pseudo
*      3D effects.
* <dt> background
* <dd> The color used for background (or inset) <I>items</I> in a drawing 
*      scheme (rather than as a typical background area per se).  For example,
*      the background of a slider (the "groove" that the thumb slides in)
*      is drawn in this color.  [This name probably needs to be changed.]
* <dt>
* <dd> Note: the colors: base, highlight, shadow, and background are designed 
*      to be related, typically appearing to be the same material but with 
*      different lighting.
* <dt> foreground
* <dd> The color normally drawn over the base color for foreground items such
*      as textual labels.  This color needs to contrast with, but not clash 
*      with the base color.
* <dt> text_background
* <dd> The color that serves as the background for text editing areas.
* <dt> splash
* <dd> A color which is designed to contrast with, and be significantly
*      different from, the base, highlight, shadow, and background color 
*      scheme.  This is used for indicators such as found inside a selected
*      radio button or check box.
* </dl>
*/




#define byte(value) (floor(value+0.5))

#define clamp constrain

PixRGBA brighter(const PixRGBA &value)
{
    uint8_t red = byte(clamp(value.red *(1/0.80), 0, 255));
    uint8_t green = byte(clamp(value.green * (1.0/0.85), 0, 255));
    uint8_t blue = byte(clamp(value.blue * (1.0/0.80), 0,255));

    return PixRGBA(red, green, blue, value.alpha);
}

PixRGBA darker(const PixRGBA &value)
{
    uint8_t red = byte(value.red *0.60);
    uint8_t green = byte(value.green * 0.60);
    uint8_t blue = byte(value.blue * 0.60);

    return PixRGBA(red, green, blue, value.alpha);
}




class GUIStyle
{
public:
    enum FrameStyle {
        Sunken = 0x01,
        Raised = 0x02
    };

protected:
    size_t fBorderWidth;
    PixRGBA fBaseColor;
    PixRGBA fForeground;
    PixRGBA fTextBackground;
    PixRGBA fHighlightColor;
    PixRGBA fShadowColor;
    PixRGBA fBackground;
    PixRGBA fBottomShadow;
    PixRGBA fBottomShadowTopLiner;
    PixRGBA fBottomShadowBottomLiner;
    PixRGBA fTopShadow;
    PixRGBA fBackground;


public:
    GUIStyle()
        : fBorderWidth(2)
    {

        fBaseColor = colors.ltGray;
        fForeground = colors.ltGray;
        fTextBackground = fBaseColor;
        fHighlightColor = brighter(fBaseColor);
        fShadowColor = darker(fBaseColor);
        fBackground = brighter(fHighlightColor);
        fBottomShadow = darker(fForeground); // 0x00616161;
        fBottomShadowTopLiner = brighter(fBottomShadow); //fForeground;
        fBottomShadowBottomLiner = fBottomShadow;
        fTopShadow = brighter(fForeground);  // 0x00cbcbcb;
        fBackground = brighter(colors.darkGray); //0x009e9e9e;
    }




    PixRGBA SunkenColor(self) {return fForeground;}


PixRGBA GUIStyle.RaisedColor(self)
	return self.fForeground;
end

PixRGBA Background(self)
	return self.fBackground;
end

PixRGBA BaseColor(self, value)
    if not value then
        return self.fBaseColor;
    end

    -- set
	self.fBaseColor = value;
	self.fTextBackground = self.fBaseColor;

    self.fHighlightColor = brighter(self.fBaseColor);
	self.fShadowColor = darker(self.fBaseColor);
	self.fBackground = brighter(self.fHighlightColor); 			
end

PixRGBA Foreground(self, value)
    -- get
    if not value then
        return self.fForeground;
    end
    
	-- set
	self.fForeground = value;
end

PixRGBA BorderWidth(self, value)
    if not value then
		return self.fBorderWidth;
    end
    
    self.fBorderWidth = value;
end

PixRGBA Padding(self)
	return 2;
end

PixRGBA DrawFrame(self, ctx, x, y, w, h, style)

	local n;

    if style == GUIStyle.FrameStyle.Sunken then
        ctx:stroke(self.fHighlightColor)
		for n=0, self:BorderWidth()-1 do
			ctx:line(x+n, y+h-n, x+w-n, y+h-n);    -- bottom shadow
            ctx:line(x + w - n, y + n, x + w - n, y + h);	    -- right shadow
        end

        ctx:stroke(self.fShadowColor)
		for n=0, self:BorderWidth()-1 do
			ctx:line(x+n, y+n, x+w-n, y+n);	    -- top edge
			ctx:line(x+n, y+n, x+n, y+h-n);	    -- left edge
        end				

    elseif style == GUIStyle.FrameStyle.Raised then	

        ctx:stroke(self.fShadowColor)
		for n=0, self:BorderWidth()-1 do
			ctx:line(x+n, y+h-n, x+w-n, y+h-n);      -- bottom shadow
			ctx:line(x+w-n, y+n, x+w-n, y+h);	    -- right shadow
        end

		if self:BorderWidth() > 0 then				
            ctx:stroke(self.fBottomShadowBottomLiner)
            ctx:line(x, y + h, x + w, y + h);				-- bottom shadow
            ctx:line(x + w, y, x + w, y + h);				-- right shadow
        end

        ctx:stroke(self.fHighlightColor)
		for n=0, self:BorderWidth()-1 do
			ctx:line(x+n,y+n, x+w-n, y+n);	    -- top edge
			ctx:line(x+n, y+n, x+n, y+h-n);	    -- left edge
        end

    end

end

PixRGBA DrawSunkenRect(self, ctx, x, y, w, h)
    ctx:fill(self.fBaseColor)
    ctx:fillRect(x,y,w,h)

	self:DrawFrame(ctx, x, y, w, h, GUIStyle.FrameStyle.Sunken);
end

PixRGBA DrawRaisedRect(self, ctx, x, y, w, h)
{
    ctx:fill(self.fBaseColor)
    ctx:fillRect(x,y,w,h)
	self:DrawFrame(ctx, x, y, w, h, GUIStyle.FrameStyle.Raised);
}

/*
        public virtual void DrawLine(IGraphPort aPort, int x1, int y1, int x2, int y2, int border_width)
		{
            // Vertical line
			if (x1 == x2)
			{
				for (int n=0; n<BorderWidth; n++) 
					aPort.DrawLine(fShadowPen, new Point2I(x1-n, y1+n), new Point2I(x1-n, y2-n));  // left part
	    
				for (int n=1; n<BorderWidth; n++) 
					aPort.DrawLine(fHighlightPen, new Point2I(x1+n, y1+n), new Point2I(x1+n, y2-n));  // right part
			} 
			else if (y1 == y2)  // Horizontal line
			{
				for (int n=0; n<BorderWidth; n++) 
					aPort.DrawLine(fShadowPen, new Point2I(x1+n, y1-n), new Point2I(x2-n, y1-n));  // top part

				for (int n=1; n<BorderWidth; n++) 
					aPort.DrawLine(fHighlightPen, new Point2I(x1+n, y1+n), new Point2I(x2-n, y1+n));  // bottom part
			}
		}
	}
}
*/

