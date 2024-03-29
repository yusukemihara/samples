/*
 * ElementBox.java
 * Copyright (c) 2005-2007 Radek Burget
 *
 * CSSBox is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *  
 * CSSBox is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *  
 * You should have received a copy of the GNU Lesser General Public License
 * along with CSSBox. If not, see <http://www.gnu.org/licenses/>.
 *
 * Created on 5. �nor 2006, 21:32
 */

package org.fit.cssbox.layout;

import java.net.MalformedURLException;
import java.net.URL;
import java.util.*;
import java.util.List;
import java.awt.*;
import java.awt.geom.Line2D;
import java.awt.image.BufferedImage;

import cz.vutbr.web.css.*;
import cz.vutbr.web.css.CSSProperty.BackgroundAttachment;
import cz.vutbr.web.css.CSSProperty.BackgroundRepeat;
import cz.vutbr.web.css.CSSProperty.ZIndex;

import org.fit.cssbox.css.CSSUnits;
import org.fit.cssbox.misc.CSSStroke;
import org.fit.net.DataURLHandler;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.w3c.dom.*;

/**
 * An abstract class representing a box formed by a DOM element. There are two
 * possible subclases: an inline box and a block box. The element box can contain
 * an arbitrary number of sub-boxes. Since the box can be split to several parts,
 * only a continuous part of the list is considered for rendering.
 * @author  radek
 */
abstract public class ElementBox extends Box
{
    private static Logger log = LoggerFactory.getLogger(ElementBox.class);
    
    public static final CSSProperty.Display DISPLAY_ANY = null;
    public static final CSSProperty.Display DISPLAY_NONE = CSSProperty.Display.NONE;
    public static final CSSProperty.Display DISPLAY_INLINE = CSSProperty.Display.INLINE;
    public static final CSSProperty.Display DISPLAY_BLOCK = CSSProperty.Display.BLOCK;
    public static final CSSProperty.Display DISPLAY_LIST_ITEM = CSSProperty.Display.LIST_ITEM;
    public static final CSSProperty.Display DISPLAY_RUN_IN = CSSProperty.Display.RUN_IN;
    public static final CSSProperty.Display DISPLAY_INLINE_BLOCK = CSSProperty.Display.INLINE_BLOCK;
    public static final CSSProperty.Display DISPLAY_TABLE = CSSProperty.Display.TABLE;
    public static final CSSProperty.Display DISPLAY_INLINE_TABLE = CSSProperty.Display.INLINE_TABLE;
    public static final CSSProperty.Display DISPLAY_TABLE_ROW_GROUP = CSSProperty.Display.TABLE_ROW_GROUP;
    public static final CSSProperty.Display DISPLAY_TABLE_HEADER_GROUP = CSSProperty.Display.TABLE_HEADER_GROUP;
    public static final CSSProperty.Display DISPLAY_TABLE_FOOTER_GROUP = CSSProperty.Display.TABLE_FOOTER_GROUP;
    public static final CSSProperty.Display DISPLAY_TABLE_ROW = CSSProperty.Display.TABLE_ROW;
    public static final CSSProperty.Display DISPLAY_TABLE_COLUMN_GROUP = CSSProperty.Display.TABLE_COLUMN_GROUP;
    public static final CSSProperty.Display DISPLAY_TABLE_COLUMN = CSSProperty.Display.TABLE_COLUMN;
    public static final CSSProperty.Display DISPLAY_TABLE_CELL = CSSProperty.Display.TABLE_CELL;
    public static final CSSProperty.Display DISPLAY_TABLE_CAPTION = CSSProperty.Display.TABLE_CAPTION;
    
    public static final CSSProperty.Position POS_STATIC = CSSProperty.Position.STATIC;
    public static final CSSProperty.Position POS_RELATIVE = CSSProperty.Position.RELATIVE;
    public static final CSSProperty.Position POS_ABSOLUTE = CSSProperty.Position.ABSOLUTE;
    public static final CSSProperty.Position POS_FIXED = CSSProperty.Position.FIXED;
    
    public static final CSSProperty.WhiteSpace WHITESPACE_NORMAL = CSSProperty.WhiteSpace.NORMAL;
    public static final CSSProperty.WhiteSpace WHITESPACE_PRE = CSSProperty.WhiteSpace.PRE;
    public static final CSSProperty.WhiteSpace WHITESPACE_NOWRAP = CSSProperty.WhiteSpace.NOWRAP;
    public static final CSSProperty.WhiteSpace WHITESPACE_PRE_WRAP = CSSProperty.WhiteSpace.PRE_WRAP;
    public static final CSSProperty.WhiteSpace WHITESPACE_PRE_LINE = CSSProperty.WhiteSpace.PRE_LINE;
    
    /** Default line height if nothing or 'normal' is specified */
    private static final float DEFAULT_LINE_HEIGHT = 1.12f;
    
    /** Assigned element */
    protected Element el;

    /** First DOM child node index covered by this box (inclusive) */
    protected int firstDOMChild;
    
    /** First DOM child node index covered by this box (exclusive) */
    protected int lastDOMChild;
    
    /** Pre-created box to be added to this box before the DOM nodes are processed. Used during the box tree creation only. */
    protected Box preadd;
    
    /** Other boxes to be added to the tree after this one. Used during the box tree creation only. */
    protected Vector<Box> postadd;
    
    /** Current DOM child during the tree creation */
    protected BoxTreeCreationStatus curstat;
    
    /** Previous copy of the same box if the box has been split */
    protected ElementBox previousTwin;
    
    /** Next copy of the same box if the box has been split */
    protected ElementBox nextTwin;
    
    /** The style of the node (for element nodes only) with no pseudo classes */
    protected NodeData style;
    
    /** Efficient styles for the pseudo classes */
    protected Map<Selector.PseudoDeclaration, NodeData> pseudoStyle;
    
    /** Background images or null when there are no background images */
    protected Vector<BackgroundImage> bgimages;
    
    /** Set to true when the element box contains only text boxes */
    protected boolean textonly;
    
    /** The map of related pseudo-elements (if any) */
    protected Map<Selector.PseudoDeclaration, ElementBox> pseudoElements;
    
    //============================== Computed style ======================
    
    /** The display property value */
    protected CSSProperty.Display display;
    
    /** Position property */
    protected CSSProperty.Position position;
    
    /** Position coordinates */
    protected LengthSet coords;
    
    /** The top position coordinate is set explicitely */
    protected boolean topset;
    
    /** The top position coordinate is set explicitely */
    protected boolean leftset;
    
    /** The top position coordinate is set explicitely */
    protected boolean bottomset;
    
    /** The top position coordinate is set explicitely */
    protected boolean rightset;
    
    /** The white-space property value */
    protected CSSProperty.WhiteSpace whitespace;
    
    /** Background color or null when transparent */
    protected Color bgcolor;

    /** Margin widths */
    protected LengthSet margin;
    
    /** Effective top and bottom margins (after collapsing with the contained boxes) */
    protected LengthSet emargin;
    
    /** Padding widths */
    protected LengthSet border;
    
    /** Border widths */
    protected LengthSet padding;
    
    /** Content sizes */
    protected Dimension content;
    
    /** Minimal absolute bounds. */
    protected Rectangle minAbsBounds;
    
    /** the computed value of line-height */
    protected int lineHeight;
    
    /** the z-index flag: true when z-index is different from <code>auto</code> */
    protected boolean zset;
    
    /** the z-index value when set */
    protected int zIndex;
    
    //============================== Child boxes ======================
    
    /** First valid child */
    protected int startChild;
    
    /** Last valid child (excl) */
    protected int endChild;

    /** A list of nested boxes (possibly empty). The box can contain either 
     * only block boxes or only inline boxes. The inline boxes can only
     * contain inline boxes */
    protected Vector<Box> nested;
    
    /** Corresponding stacking context if this box creates one. */
    protected StackingContext scontext;
    
    //=======================================================================
    
    /**
     * Creates a new element box from a DOM element
     * @param n the DOM element
     * @param g current graphics context
     * @param ctx current visual context
     */
    public ElementBox(Element n, Graphics2D g, VisualContext ctx)
    {
        super(n, g, ctx);
        minAbsBounds = null;
        style = null;
        pseudoStyle = new HashMap<Selector.PseudoDeclaration, NodeData>();
        if (n != null)
        {
	        el = n;
	        firstDOMChild = 0;
	        lastDOMChild = n.getChildNodes().getLength();
	        previousTwin = null;
	        nextTwin = null;
	        
	        nested = new Vector<Box>();
	        pseudoElements = new HashMap<Selector.PseudoDeclaration, ElementBox>();
	        startChild = 0;
	        endChild = 0;
	        isblock = false;
	        textonly = true;
        }
        position = POS_STATIC;
        topset = false;
        leftset = false;
        bottomset = false;
        rightset = false;
    }
    
    /**
     * Copy the values from another element box.
     * @param src source element box
     */ 
    public void copyValues(ElementBox src)
    {
        super.copyValues(src);
        nested.addAll(src.nested);
        textonly = src.textonly;
        pseudoElements = new HashMap<Selector.PseudoDeclaration, ElementBox>(src.pseudoElements);
        style = src.style; 
        pseudoStyle = new HashMap<Selector.PseudoDeclaration, NodeData>(src.pseudoStyle);
        startChild = src.startChild;
        endChild = src.endChild;
        isblock = src.isblock;
        style = src.style;
        display = src.display;
        lineHeight = src.lineHeight;
        whitespace = src.whitespace;
        bgcolor = (src.bgcolor == null) ? null : new Color(src.bgcolor.getRed(), src.bgcolor.getGreen(), src.bgcolor.getBlue(), src.bgcolor.getAlpha());
        bgimages = (src.bgimages == null) ? null : new Vector<BackgroundImage>(src.bgimages);
        position = src.position;
        topset = src.topset;
        leftset = src.leftset;
        bottomset = src.bottomset;
        rightset = src.rightset;
        
        if (src.coords != null)
            coords = new LengthSet(src.coords);
        
        if (src.margin != null)
            margin = new LengthSet(src.margin);
        if (src.emargin != null)
            emargin = new LengthSet(src.emargin);
        if (src.border != null)
            border = new LengthSet(src.border);
        if (src.padding != null)
            padding = new LengthSet(src.padding);
        if (src.content != null)
            content = new Dimension(src.content);
    }
    
    /** Create a new box from the same DOM node in the same context */
    abstract public ElementBox copyBox();
    
    @Override
    public void initSubtree()
    {
        initBox();
        loadSizes();
        
        for (int i = 0; i < getSubBoxNumber(); i++)
            getSubBox(i).initSubtree();
        
        computeEfficientMargins();
    }
    
    //=======================================================================
    
    /**
     * @return the corresponding DOM element
     */
    public Element getElement()
    {
        return el;
    }
    
    /**
     * Assign a new style to this box
     * @param s the new style declaration
     */
    public void setStyle(NodeData s)
    {
    	style = s;
    	loadBasicStyle();
    }
    
    /**
     * Returns the style of the DOM node that forms this box.
     * @return the style declaration
     */
    public NodeData getStyle()
    {
        return style;
    }
    
    /**
     * Obtains the string representation of the current style of the box in the CSS syntax.
     * @return The style string representation.
     */
    public String getStyleString()
    {
        if (style != null)
            return style.toString();
        else
            return "";
    }
    
    /**
     * Obtains the string value of the position: property
     * @return The string like "static", "absolute", "relative" or "fixed"
     */
    public String getPositionString()
    {
        return position.toString();
    }
    
    /**
     * Reads the string value of the specified property of the element style.
     * @param property the property name
     * @return the property value
     */
    public String getStylePropertyValue(String property)
    {
        Object t = style.getValue(Term.class, property);
        if (t == null)
            return "";
        else
            return t.toString();
    }
    
    /**
     * Reads a CSS length value of a style property of the box.
     * @param name property name
     * @return the length value
     */
    public TermLengthOrPercent getLengthValue(String name)
    {
        if (style != null)
            return style.getValue(TermLengthOrPercent.class, name);
        else
            return null;
    }
    
    /**
     * Reads the value of a border width specified by a CSS property.
     * @param dec a CSS decoder used for converting the values
     * @param property the property name, e.g. "border-top-width"
     * @return the border width in pixels
     */
    public int getBorderWidth(CSSDecoder dec, String property)
    {
        if (style != null)
        {
            CSSProperty.BorderWidth prop = style.getProperty(property);
            if (prop == CSSProperty.BorderWidth.length)
                return dec.getLength(style.getValue(TermLengthOrPercent.class, property), false, CSSUnits.MEDIUM_BORDER, 0, 0);
            else
                return CSSUnits.convertBorderWidth(prop);
        }
        else
            return 0;
    }
    
    /**
     * Returns the value of the display property
     * @return One of the ElementBox.DISPLAY_XXX constants
     */
    public CSSProperty.Display getDisplay()
    {
    	return display;
    }
    
    public String getDisplayString()
    {
        if (display != null)
            return display.toString();
        else
            return "";
    }
    
    /**
     * Returns the value of the white-space property
     * @return one of the ElementBox.WHITESPACE_XXX constants
     */
    public CSSProperty.WhiteSpace getWhiteSpace()
    {
        return whitespace;
    }
    
    /**
     * Checks whether the whitespaces should be collapsed within in the element according to its style.
     * @return <code>true</code> if the whitespace sequences should be collapsed.
     */
    @Override
    public boolean collapsesSpaces()
    {
        return (whitespace != WHITESPACE_PRE && whitespace != WHITESPACE_PRE_WRAP);
    }
    
    /**
     * Checks whether this element should preserve the line breaks according to its style.
     * @return <code>true</code> when the line breaks should be preserved
     */
    @Override
    public boolean preservesLineBreaks()
    {
        return (whitespace != WHITESPACE_NORMAL && whitespace != WHITESPACE_NOWRAP);
    }
    
    /**
     * Checks whether this box allows line wrapping on whitespaces according to the whit-space setting.
     * @return <code>true</code> when line wrapping is allowed
     */
    @Override
    public boolean allowsWrapping()
    {
        return (whitespace == ElementBox.WHITESPACE_NORMAL
                || whitespace == ElementBox.WHITESPACE_PRE_WRAP
                || whitespace== ElementBox.WHITESPACE_PRE_LINE);
    }
    
    /**
     * Obtains the efficient background color to be used for drawing the background.
     * @return the background color or null when transparent
     */
    public Color getBgcolor()
    {
        return bgcolor;
    }
    
    /**
     * Obtains the list of background images of the element.
     * @return a list of the background images
     */
    public List<BackgroundImage> getBackgroundImages()
    {
        return bgimages;
    }

    /**
     * @param bgcolor the background color
     */
    public void setBgcolor(Color bgcolor)
    {
        this.bgcolor = bgcolor;
    }

    /**
     * @return the number of subboxes in this box
     */
    public int getSubBoxNumber()
    {
        return nested.size();
    }

    /**
     * @return list of all the subboxes 
     */
    public List<Box> getSubBoxList()
    {
        return nested;
    }
    
    /**
     * @param index the sub box index in the range from 0 to n-1 
     * @return the appropriate sub box
     */
    public Box getSubBox(int index)
    {
        return nested.elementAt(index);
    }
    
    /**
     * Adds a new sub box to the end of the sub box list.
     * @param box the new sub box to add
     */
    public void addSubBox(Box box)
    {
        box.setParent(this);
        nested.add(box);
        endChild++;
        if (isDisplayed() && !box.isEmpty())
            isempty = false;
        if (!(box instanceof TextBox))
            textonly = false;
    }
    
    /**
     * Removes a sub box from the subbox list
     * @param box the new sub box to add
     */
    public void removeSubBox(Box box)
    {
        if (nested.remove(box))
            endChild--;
    }
    
    /**
     * Removes all sub boxes from the subbox list
     */
    public void removeAllSubBoxes()
    {
        nested.removeAllElements();
        endChild = 0;
    }
    
    /**
     * Inserts a new sub box before a specified sub box
     * @param where the box already existing in the list
     * @param what the new box to add
     */
    public void insertSubBoxBefore(Box where, Box what)
    {
        int pos = nested.indexOf(where);
        nested.insertElementAt(what, pos);
        endChild++;
    }

    /**
     * Inserts a new sub box after a specified sub box
     * @param where the box already existing in the list
     * @param what the new box to add
     */
    public void insertSubBoxAfter(Box where, Box what)
    {
        int pos = nested.indexOf(where);
        nested.insertElementAt(what, pos+1);
        endChild++;
    }

    /**
     * Inserts a new sub box at a specified index
     * @param index the index where the new box will be placed
     * @param what the new box to add
     */
    public void insertSubBox(int index, Box what)
    {
        nested.insertElementAt(what, index);
        endChild++;
    }
    
    /**
     * Obtains the stacking context if this box creates one.
     */
    public StackingContext getStackingContext()
    {
        if (scontext == null)
        {
            if (formsStackingContext())
                scontext = new StackingContext(this);
            else
                log.warn("getStackingContext: obtaining a stacking context from element that does not create one");
        }
        return scontext;
    }
    
    /**
     * Sets related pseudo-element boxes
     * @param pseudo the name of the pseudo-element
     * @param box the corresponding pseudo-element box
     */
    public void setPseudoElement(Selector.PseudoDeclaration pseudo, ElementBox box)
    {
        pseudoElements.put(pseudo, box);
    }
    
    /**
     * Gets the list of related pseudo-element boxes
     * @param pseudo the name of the pseudo-element
     * @return the related box
     */
    public ElementBox getPseudoElement(Selector.PseudoDeclaration pseudo)
    {
        return pseudoElements.get(pseudo);
    }
    
    /**
     * Checks whether the element box has a related pseudo-element box
     * @param pseudo the name of the pseudo-element
     * @return the related box
     */
    public boolean hasPseudoElement(Selector.PseudoDeclaration pseudo)
    {
        return pseudoElements.containsKey(pseudo);
    }
    
    /**
     * @return the width of the content without any margins and borders
     */
    public int getContentWidth()
    {
    	return content.width;
    }
    
    /**
     * @return the height of the content without any margins and borders
     */
    public int getContentHeight()
    {
    	return content.height;
    }
    
    /**
     * Obtains the computed value of the declared line height of the element.
     * @return the line height in pixels
     */
    public int getLineHeight()
    {
        return lineHeight;
    }

    /**
     * @return the margin sizes
     */
    public LengthSet getMargin()
    {
        return margin;
    }
    
    /**
     * @return the effective margin sizes (after collapsing)
     */
    public LengthSet getEMargin()
    {
        return emargin;
    }
    
    /**
     * Obtains the position coordinates (top, left, bottom, right properties)
     * @return the set of coordinates
     */
    public LengthSet getCoords()
    {
        return coords;
    }
    
    /**
     * @return the border sizes (0 when no border is displayed)
     */
    public LengthSet getBorder()
    {
        return border;
    }
    
    /**
     * @return the padding sizes
     */
    public LengthSet getPadding()
    {
        return padding;
    }
    
    /**
     * @return the content sizes
     */
    public Dimension getContent()
    {
        return content;
    }
    
    /**
     * Checks whether the z-index is non-auto for this box.
     * @return <code>true</code> when z-index has a value different form <code>auto</code>.
     */
    public boolean hasZIndex()
    {
        return zset;
    }
    
    /**
     * Obtains the declared z-index for this element.
     * @return the z-index value
     */
    public int getZIndex()
    {
        return zIndex;
    }
    
    /**
     * Check whether the element forms a new stacking context.
     * @return <code>true</code> when the element creates a new stacking context.
     */
    public boolean formsStackingContext()
    {
        return position != POS_STATIC;
    }
    
    /**
     * @return the first child from the list that is considered for rendering
     */
    public int getStartChild()
    {
        return startChild;
    }
    
    /**
     * @param index the index of the first child from the list that is considered for rendering
     */
    public void setStartChild(int index)
    {
        startChild = index;
    }
    
    /**
     * @return the last child from the list that is considered for rendering (not included)
     */
    public int getEndChild()
    {
        return endChild;
    }
    
    /**
     * @param index the index of the last child from the list that is considered for rendering
     */
    public void setEndChild(int index)
    {
        endChild = index;
    }
    
    /**
     * Sets the parent of the valid children to this (used while splitting the boxes)
     */
    public void adoptChildren()
    {
        for (int i = startChild; i < endChild; i++)
            nested.elementAt(i).setParent(this);
    }
    
    //=======================================================================

    /**
     * Computes the distance of the content from the left edge of the whole box
     * (a convenience function for margin + border + padding).
     * @return the distance 
     */
    public int getContentOffsetX()
    {
        return margin.left + border.left + padding.left;
    }
    
    /**
     * Computes the distance of the content from the top edge of the whole box. 
     * (a convenience function for margin + border + padding).
     * @return the distance
     */
    public int getContentOffsetY()
    {
        return emargin.top + border.top + padding.top;
    }
    
    public int getContentX()
    {
        return bounds.x + emargin.left + border.left + padding.left;
    }
    
    public int getContentY()
    {
        return bounds.y + emargin.top + border.top + padding.top;
    }
    
    public int getAbsoluteContentX()
    {
        return absbounds.x + emargin.left + border.left + padding.left;
    }
    
    public int getAbsoluteContentY()
    {
        return absbounds.y + emargin.top + border.top + padding.top;
    }
    
    public int totalWidth()
    {
        return emargin.left + border.left + padding.left + content.width +
            padding.right + border.right + emargin.right;
    }
    
    //totalHeight() differs for inline and block boxes
    
    public int getAvailableContentWidth()
    {
        return availwidth - emargin.left - border.left - padding.left 
                  - padding.right - border.right - emargin.right;
    }
    
    @Override
    public Rectangle getMinimalAbsoluteBounds()
    {
    	if (minAbsBounds == null)
    		minAbsBounds = computeMinimalAbsoluteBounds();
    	return minAbsBounds;
    }
    
    private Rectangle computeMinimalAbsoluteBounds()
    {
    	int rx1 = 0, ry1 = 0, rx2 = 0, ry2 = 0;
    	boolean valid = false;
    	for (int i = startChild; i < endChild; i++)
		{
			Box sub = getSubBox(i);
			Rectangle sb = sub.getMinimalAbsoluteBounds();
			if (sub.isDisplayed() && sub.isVisible() && sb.width > 0 && sb.height > 0)
			{
				if (sb.x < rx1 || !valid) rx1 = sb.x;
				if (sb.y < ry1 || !valid) ry1 = sb.y;
				if (sb.x + sb.width > rx2 || !valid) rx2 = sb.x + sb.width;
				if (sb.y + sb.height > ry2 || !valid) ry2 = sb.y + sb.height;
				valid = true;
			}
		}
    	return new Rectangle(rx1, ry1, rx2 - rx1, ry2 - ry1);
    }
    
    @Override
    public boolean affectsDisplay()
    {
        boolean ret = !isEmpty();
        //non-zero top or bottom border
        if (border.top > 0 || border.bottom > 0)
            ret = true;
        //the same with padding
        if (padding.top > 0 || padding.bottom > 0)
            ret = true;
        
        return ret;
    }
    
    @Override
    public boolean isVisible()
    {
        return visible && clipblock.isVisible() && clipblock.visibleInClip(this);
    }
    
    /**
     * Checks whether a box is visible in the content area of this box when this is a clipping block.
     * @param box The box to test.
     * @return {@code true} when at least a part of the given box is inside of our clipping area.
     */
    public boolean visibleInClip(Box box)
    {
        if (box instanceof ElementBox)
        {
            //the margin is never visible - use the border bounds instead of the full bounds
            return getClippedContentBounds().intersects(((ElementBox) box).getAbsoluteBorderBounds());
        }
        else
        {
            return getClippedContentBounds().intersects(box.getAbsoluteBounds());
        }
    }
    
    /**
     * Returns the bounds of the padding edge (the content and padding)
     * @return a Rectangle representing the absolute padding bounds
     */
    public Rectangle getAbsolutePaddingBounds()
    {
        return new Rectangle(absbounds.x + emargin.left + border.left,
                             absbounds.y + emargin.top + border.top,
                             content.width + padding.left + padding.right,
                             content.height + padding.top + padding.bottom);
    }

    /**
     * Returns the bounds of the border edge (the content, padding and border)
     * @return a Rectangle representing the absolute border bounds
     */
    public Rectangle getAbsoluteBorderBounds()
    {
        return new Rectangle(absbounds.x + emargin.left,
                             absbounds.y + emargin.top,
                             content.width + padding.left + padding.right + border.left + border.right,
                             content.height + padding.top + padding.bottom + border.top + border.bottom);
    }
    
    /**
     * Returns the bounds of the background according to the background-origin property.
     * NOTE: currently, background-origin is not supported i.e. this method is equals to getAbsolutePaddingBounds().
     * This will change in the future.
     * @return a Rectangle representing the absolute background bounds
     */
    public Rectangle getAbsoluteBackgroundBounds()
    {
        return getAbsolutePaddingBounds(); //TODO change this when background-origin is supported
    }
    
    /**
     * Computes the absolute clipping rectangle coordinates if this box is used as a clipping block.
     * @return the clipping rectangle coordinates
     */
    public Rectangle getClippedContentBounds()
    {
        if (clipblock == null)
            return getAbsolutePaddingBounds();
        else
            return clipblock.getClippedContentBounds().intersection(getAbsolutePaddingBounds());
    }
    
    @Override
    public String getText()
    {
        String ret = "";
        for (int i = startChild; i < endChild; i++)
        	ret += getSubBox(i).getText();
        return ret;
    }
    
    @Override
    public boolean isWhitespace()
    {
        for (int i = startChild; i < endChild; i++)
            if (!getSubBox(i).isWhitespace())
                return false;
        return true;
    }
        
    /**
     * Checks if the element contains only text boxes (no nested elements)
     * @return <code>true</code> when only text boxes are contained in this element
     */
    public boolean containsTextOnly()
    {
        for (Box child : nested)
            if (!(child instanceof TextBox))
                return false;
        return true;
    }
    
    /**
     * Checks if the element contains the mix of text boxes and elements
     * @return <code>true</code> when both text boxes and elements are directly contained in this element
     */
    public boolean containsMixedContent()
    {
        boolean ctext = false;
        boolean celem = false;
        for (Box child : nested)
        {
            if (child instanceof TextBox)
                ctext = true;
            else
                celem = true;
            
            if (ctext && celem)
                return true;
        }
        return false;
    }
    
    //=======================================================================
    
    /**
     * Draws the subtree as if this was a stacking context root.
     * @param include include new stacking contexts to this context (currently unused)
     */
    public void drawStackingContext(boolean include)
    {
        if (isDisplayed() && isDeclaredVisible())
        {
            Integer[] clevels = formsStackingContext() ? getStackingContext().getZIndices() : new Integer[0]; 
            
            //1.the background and borders of the element forming the stacking context.
            if (this.formsStackingContext())
                getViewport().getRenderer().renderElementBackground(this);
            
            getViewport().getRenderer().startElementContents(this);
            
            if (this.isReplaced() && this.formsStackingContext())
                getViewport().getRenderer().renderReplacedContent((ReplacedBox) this);
            
            //2.the child stacking contexts with negative stack levels (most negative first).
            int zi = 0;
            while (zi < clevels.length && clevels[zi] < 0)
            {
                drawChildContexts(clevels[zi]);
                zi++;
            }
            //3.the in-flow, non-inline-level, non-positioned descendants.
            drawChildren(DrawStage.DRAW_NONINLINE);
            //4.the non-positioned floats. 
            drawChildren(DrawStage.DRAW_FLOAT);
            //5.the in-flow, inline-level, non-positioned descendants, including inline tables and inline blocks. 
            drawChildren(DrawStage.DRAW_INLINE);
            //6.the child stacking contexts with stack level 0 and the positioned descendants with stack level 0.
            if (zi < clevels.length && clevels[zi] == 0)
            {
                drawChildContexts(0);
                zi++;
            }
            //7.the child stacking contexts with positive stack levels (least positive first).
            while (zi < clevels.length)
            {
                drawChildContexts(clevels[zi]);
                zi++;
            }
            
            getViewport().getRenderer().finishElementContents(this);
            
        }
    }
    
    /**
     * Draws all the sub-boxes in the given stage.
     * @param turn The current drawing stage.
     */
    protected void drawChildren(DrawStage turn)
    {
        for (int i = startChild; i < endChild; i++)
        {
            Box subbox = getSubBox(i);
            subbox.draw(turn);
        }
    }
    
    /**
     * Draws child stacking contexts of the given z-index.
     * @param zindex The z-index of the contexts to draw. Only the child contexts with the given z-index will be drawn.
     */
    protected void drawChildContexts(int zindex)
    {
        Vector<ElementBox> list = getStackingContext().getElementsForZIndex(zindex);
        if (list != null)
        {
            for (ElementBox elem : list)
            {
                elem.drawStackingContext(!elem.hasZIndex());
            }
        }
    }
    
    /** 
     * Draw the background and border of this box (no subboxes).
     * This method is normally called automatically from {@link Box#draw(DrawStage)}.
     * @param g the graphics context used for drawing 
     */
    public void drawBackground(Graphics2D g)
    {
        Color color = g.getColor(); //original color
        Shape oldclip = g.getClip(); //original clip region
        if (clipblock != null)
            g.setClip(applyClip(oldclip, clipblock.getClippedContentBounds()));
        ctx.updateGraphics(g);
        
        //top left corner
        int x = absbounds.x;
        int y = absbounds.y;

        //border bounds
        int bx1 = x + margin.left;
        int by1 = y + emargin.top;
        int bw = border.left + padding.left + content.width + padding.right + border.right;
        int bh = border.top + padding.top + content.height + padding.bottom + border.bottom;
        int bx2 = bx1 + bw - 1;
        int by2 = by1 + bh - 1;
        
        //draw the background - it should be visible below the border too
        if (getBgcolor() != null)
        {
            g.setColor(getBgcolor());
            g.fillRect(bx1, by1, bw, bh);
        }
        
        //draw the background images
        if (bgimages != null)
        {
            for (BackgroundImage img : bgimages)
            {
                //img.draw(g, 0, 0);
                BufferedImage bimg = img.getBufferedImage();
                if (bimg != null)
                    g.drawImage(bimg, bx1 + border.left, by1 + border.top, null);
            }
        }
        
        //draw the border
        drawBorders(g, bx1, by1, bx2, by2);
        
        g.setClip(oldclip); //restore the clipping
        g.setColor(color); //restore original color
    }
    
    protected void drawBorders(Graphics2D g, int bx1, int by1, int bx2, int by2)
    {
        if (border.top > 0 && bx2 > bx1)
            drawBorder(g, bx1, by1, bx2, by1, border.top, 0, 0, "top", false);
        if (border.right > 0 && by2 > by1)
            drawBorder(g, bx2, by1, bx2, by2, border.right, -border.right + 1, 0, "right", true); 
        if (border.bottom > 0 && bx2 > bx1)
            drawBorder(g, bx1, by2, bx2, by2, border.bottom, 0, -border.bottom + 1, "bottom", true); 
        if (border.left > 0 && by2 > by1)
            drawBorder(g, bx1, by1, bx1, by2, border.left, 0, 0, "left", false); 
    }
    
    private void drawBorder(Graphics2D g, int x1, int y1, int x2, int y2, int width, 
                            int right, int down, String side, boolean reverse)
    {
        CSSProperty.BorderColor bclr = style.getProperty("border-"+side+"-color");
        TermColor tclr = style.getValue(TermColor.class, "border-"+side+"-color");
        CSSProperty.BorderStyle bst = style.getProperty("border-"+side+"-style");
        if (bst != CSSProperty.BorderStyle.HIDDEN && bclr != CSSProperty.BorderColor.TRANSPARENT)
        {
            //System.out.println("Elem: " + this + "side: " + side + "color: " + tclr);
            Color clr = null;
            if (tclr != null)
                clr = tclr.getValue();
            if (clr == null)
            {
                clr = ctx.getColor();
                if (clr == null)
                    clr = Color.BLACK;
            }
            g.setColor(clr);
            g.setStroke(new CSSStroke(width, bst, reverse));
            g.draw(new Line2D.Double(x1 + right, y1 + down, x2 + right, y2 + down));
        }
    }

    @Override
    public void drawExtent(Graphics2D g)
    {
    	//draw the full box
        g.setColor(Color.RED);
        g.drawRect(absbounds.x, absbounds.y, bounds.width, bounds.height);
    	
    	//draw the content box
        g.setColor(Color.ORANGE);
        g.drawRect(getAbsoluteContentX(), getAbsoluteContentY(), getContentWidth(), getContentHeight());
        
        //draw the real content box
        /*g.setColor(Color.GREEN);
        Rectangle r = getMinimalBounds();
        g.drawRect(r.x, r.y, r.width, r.height);*/
    }
    
    //=======================================================================
    
    /**
     * @return <code> true when this box may contain block boxes (it may be a containing-block)
     */
    abstract public boolean mayContainBlocks();
    
    /**
     * Load the box sizes from the CSS properties.
     */
    abstract protected void loadSizes();
    
    /**
     * Update the box sizes according to the new parent size. Should be
     * called when the parent has been resized.
     */
    abstract public void updateSizes(); 
    
    /**
     * Re-calculates the sizes of all the child block boxes recursively.
     */
    public void updateChildSizes()
    {
        for (int i = 0; i < getSubBoxNumber(); i++)
        {
            Box child = getSubBox(i);
            if (child instanceof BlockBox)
            {
                //update the children only when some size has changed
                BlockBox block = (BlockBox) child;
                int oldw = block.getContentWidth();
                int oldh = block.getContentHeight();
                block.updateSizes();
                block.setSize(block.totalWidth(), block.totalHeight());
                if (block.getContentWidth() != oldw || block.getContentHeight() != oldh)
                {
                    block.updateChildSizes();
                }
            }
            else if (child instanceof ElementBox)
            {
                //always update the non-block elements
                ElementBox eb = (ElementBox) child;
                eb.updateSizes();
                eb.setSize(eb.totalWidth(), eb.totalHeight());
                eb.updateChildSizes();
            }
        }
    }
    
    /**
     * Computes efficient top and bottom margins for collapsing.
     */
    abstract public void computeEfficientMargins();
    
    /**
     * Checks if the element's own top and bottom margins are adjoining 
     * according to the CSS specifiaction.
     * @return <code>true</code> if the margins are adjoining
     */
    abstract public boolean marginsAdjoin();
    
    /**
     * @return <code>true</code> if the box has a visible border around
     */
    protected boolean borderVisible(String dir)
    {
        CSSProperty.BorderStyle st = style.getProperty("border-"+dir+"-style");
        return (st != null && st != CSSProperty.BorderStyle.NONE  && st != CSSProperty.BorderStyle.HIDDEN); 
    }
    
    /**
     * Loads the border sizes from the style.
     * 
     * @param dec CSS decoder used for decoding the style
     * @param contw containing block width for decoding percentages
     */
    protected void loadBorders(CSSDecoder dec, int contw)
    {
        border = new LengthSet();
        if (borderVisible("top"))
            border.top = getBorderWidth(dec, "border-top-width");
        else
            border.top = 0;
        if (borderVisible("right"))
            border.right = getBorderWidth(dec, "border-right-width");
        else
            border.right = 0;
        if (borderVisible("bottom"))
            border.bottom = getBorderWidth(dec, "border-bottom-width");
        else
            border.bottom = 0;
        if (borderVisible("left"))
            border.left = getBorderWidth(dec, "border-left-width");
        else
            border.left = 0;
    }
    
    /**
     * Load the basic style from the CSS properties. This includes the display
     * properties, floating, positioning, color and font properties.
     */
    protected void loadBasicStyle()
    {
        ctx.updateForGraphics(style, g);
        
        display = style.getProperty("display");
        if (display == null) display = CSSProperty.Display.INLINE;
        
        CSSProperty.Float floating = style.getProperty("float");
        if (floating == null) floating = BlockBox.FLOAT_NONE;
        
        position = style.getProperty("position");
        if (position == null) position = BlockBox.POS_STATIC;
        
        //apply combination rules
        //http://www.w3.org/TR/CSS21/visuren.html#dis-pos-flo
        if (display == ElementBox.DISPLAY_NONE)
        {
            position = BlockBox.POS_STATIC;
            floating = BlockBox.FLOAT_NONE;
        }
        else if (position == BlockBox.POS_ABSOLUTE || position == BlockBox.POS_FIXED)
        {
            floating = BlockBox.FLOAT_NONE;
        }
        //compute the display computed value
        if (floating != BlockBox.FLOAT_NONE || position == BlockBox.POS_ABSOLUTE || position == BlockBox.POS_FIXED || isRootElement())
        {
            if (display == DISPLAY_INLINE_TABLE)
                display = DISPLAY_TABLE;
            else if (display == DISPLAY_INLINE ||
                     display == DISPLAY_RUN_IN ||
                     display == DISPLAY_TABLE_ROW_GROUP ||
                     display == DISPLAY_TABLE_COLUMN ||
                     display == DISPLAY_TABLE_COLUMN_GROUP ||
                     display == DISPLAY_TABLE_HEADER_GROUP ||
                     display == DISPLAY_TABLE_FOOTER_GROUP ||
                     display == DISPLAY_TABLE_ROW ||
                     display == DISPLAY_TABLE_CELL ||
                     display == DISPLAY_TABLE_CAPTION ||
                     display == DISPLAY_INLINE_BLOCK)
                display = DISPLAY_BLOCK;
        }

        isblock = (display == DISPLAY_BLOCK);
        displayed = (display != DISPLAY_NONE && display != DISPLAY_TABLE_COLUMN);
        visible = (style.getProperty("visibility") != CSSProperty.Visibility.HIDDEN); 
        
        //line height
        CSSProperty.LineHeight lh = style.getProperty("line-height");
        if (lh == null || lh == CSSProperty.LineHeight.NORMAL)
            lineHeight = Math.round(DEFAULT_LINE_HEIGHT * ctx.getFontHeight());
        else if (lh == CSSProperty.LineHeight.length)
        {
            TermLength len = style.getValue(TermLength.class, "line-height");
            lineHeight = (int) ctx.pxLength(len);
        }
        else if (lh == CSSProperty.LineHeight.percentage)
        {
            TermPercent len = style.getValue(TermPercent.class, "line-height");
            lineHeight = (int) ctx.pxLength(len, ctx.getFontHeight()); 
        }
        else //must be INTEGER or NUMBER
        {
            Term<?> len = style.getValue("line-height", true);
            float r;
            if (len instanceof TermInteger)
                r = ((TermInteger) len).getValue();
            else
                r = ((TermNumber) len).getValue();
            lineHeight = Math.round(r * ctx.getFontHeight());
        }

        //whitespace
        whitespace = style.getProperty("white-space");
        if (whitespace == null) whitespace = WHITESPACE_NORMAL;
        
        //background
        loadBackground();
        
        //z-index
        CSSProperty.ZIndex z = style.getProperty("z-index");
        if (z != null && z != ZIndex.AUTO)
        {
            zset = true;
            Term<?> zterm = style.getValue("z-index", true);
            if (zterm instanceof TermInteger)
                zIndex = ((TermInteger) zterm).getValue().intValue();
            else
                zset = false;
        }
        else
            zset = false;
    }
    
    /**
     * Loads the background information from the style
     */
    protected void loadBackground()
    {
        CSSProperty.BackgroundColor bg = style.getProperty("background-color");
        if (bg == CSSProperty.BackgroundColor.color)
        {
            TermColor bgc = style.getValue(TermColor.class, "background-color");
            bgcolor = bgc.getValue();
        }
        else
            bgcolor = null;

        bgimages = loadBackgroundImages(style);
    }
    
    /**
     * Loads background images from style. Considers their positions, repetition, etc. Currently, only a single background
     * image is supported (CSS2).
     * @param style the style containing the image specifiations
     * @return a list of images
     */
    protected Vector<BackgroundImage> loadBackgroundImages(NodeData style)
    {
        CSSProperty.BackgroundImage img = style.getProperty("background-image");
        if (img == CSSProperty.BackgroundImage.uri)
        {
            try {
                Vector<BackgroundImage> bgimages = new Vector<BackgroundImage>(1);
                TermURI urlstring = style.getValue(TermURI.class, "background-image");
                URL url = DataURLHandler.createURL(urlstring.getBase(), urlstring.getValue());
                CSSProperty.BackgroundPosition position = style.getProperty("background-position");
                TermList positionValues = style.getValue(TermList.class, "background-position");
                CSSProperty.BackgroundRepeat repeat = style.getProperty("background-repeat");
                if (repeat == null) repeat = BackgroundRepeat.REPEAT;
                CSSProperty.BackgroundAttachment attachment = style.getProperty("background-attachment");
                if (attachment == null) attachment = BackgroundAttachment.SCROLL;
                BackgroundImage bgimg = new BackgroundImage(this, url, position, positionValues, repeat, attachment);
                bgimages.add(bgimg);
                return bgimages;
            } catch (MalformedURLException e) {
                log.warn(e.getMessage());
                return null;
            }
        }
        else
            return null;
    }
    
    /**
     * Loads the top, left, bottom and right coordinates from the style
     */
    protected void loadPosition()
    {
        CSSDecoder dec = new CSSDecoder(ctx);

        int contw = cblock.getContentWidth();
        int conth = cblock.getContentHeight();
        
        CSSProperty.Top ptop = style.getProperty("top");
        CSSProperty.Right pright = style.getProperty("right");
        CSSProperty.Bottom pbottom = style.getProperty("bottom");
        CSSProperty.Left pleft = style.getProperty("left");

        topset = !(ptop == null || ptop == CSSProperty.Top.AUTO);
        rightset = !(pright == null || pright == CSSProperty.Right.AUTO);
        bottomset = !(pbottom == null || pbottom == CSSProperty.Bottom.AUTO);
        leftset = !(pleft == null || pleft == CSSProperty.Left.AUTO);
        
        coords = new LengthSet();
        if (topset)
            coords.top = dec.getLength(getLengthValue("top"), (ptop == CSSProperty.Top.AUTO), 0, 0, conth);
        if (rightset)
            coords.right = dec.getLength(getLengthValue("right"), (pright == CSSProperty.Right.AUTO), 0, 0, contw);
        if (bottomset)
            coords.bottom = dec.getLength(getLengthValue("bottom"), (pbottom == CSSProperty.Bottom.AUTO), 0, 0, conth);
        if (leftset)
            coords.left = dec.getLength(getLengthValue("left"), (pleft == CSSProperty.Left.AUTO), 0, 0, contw);
    }
    
    /**
     * Updates the stacking parent values and registers the z-index for this parent.
     */
    @Override
    protected void updateStackingContexts()
    {
        super.updateStackingContexts();
        if (stackingParent != null)
        {
            if (position != POS_STATIC) //all the positioned boxes are considered as separate stacking contexts
            {
                stackingParent.getStackingContext().registerChildContext(this);
                if (scontext != null) //clear this context if it exists (remove old children)
                    scontext.clear();
            }
        }
    }
    
}
