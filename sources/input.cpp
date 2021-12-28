#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

#include "head_file.h"




class FocusObject
{
public:
    virtual void setFocus() = 0;
    virtual void deleteFocus() = 0;
    virtual void event(const sf::Event&) = 0;
};


class TextBox : public sf::Drawable, public FocusObject, public sf::Transformable 
{
private:
    const int MAXS_LEN = 30;
    sf::RectangleShape m_outline_box;
    sf::Font m_font;
    mutable sf::RectangleShape m_box;
    mutable sf::Text m_text;
    mutable sf::String m_newText;
    mutable sf::Text m_tb_text;
    mutable bool m_textChanged;
    mutable bool m_decrease;

    void updateRect()
    {
        sf::FloatRect rect = m_text.getGlobalBounds();
        if (!m_decrease)
        {
            m_box.setPosition(rect.left - 5, rect.top - 5);
            m_box.setSize(sf::Vector2f(rect.width + 10, rect.height));
        }
        else
        {
            m_decrease = false;
        }
    }
    

public:
    std::string m_str_text;
    TextBox(const sf::Text& text, sf::Font font) : m_text(text), m_str_text(text.getString().toAnsiString()), m_font(font)
    {
        m_newText = m_text.getString();
        m_decrease = false; // ”дал€ют ли текст или нет
        
        
        m_outline_box.setPosition(5, 50);
        m_outline_box.setSize(sf::Vector2f(25 * MAXS_LEN, 40));
        m_outline_box.setOutlineThickness(1);
        m_outline_box.setOutlineColor(sf::Color::Black);

        m_box.setFillColor(sf::Color::White);

        m_tb_text.setString("y =");
        m_tb_text.setPosition(10, 50);
        m_tb_text.setFont(m_font);
        m_tb_text.setFillColor(sf::Color::Black);
        updateRect();
    }
    virtual void draw(sf::RenderTarget& render, sf::RenderStates states) const
    {
        if (m_textChanged)
        {
            const_cast<TextBox*>(this)->setText(m_newText);

            m_textChanged = false;
        }
        render.draw(m_outline_box, states);
        render.draw(m_box, states);
        render.draw(m_text, states);
        render.draw(m_tb_text, states);
        
    }
    virtual void setFocus()
    {
        m_box.setFillColor(sf::Color::White);
    }
    virtual void deleteFocus()
    {
        m_box.setFillColor(sf::Color::Black);
    }
    virtual void event(const sf::Event& event)
    {
        if (event.type == sf::Event::TextEntered)
        {
            
            //ќбработка ввода
            m_textChanged = true;
            switch (event.text.unicode)
            {
            case 0x8://Backspace
                if (!m_newText.isEmpty())
                {
                    m_decrease = true;
                    m_newText.erase(m_newText.getSize() - 1);
                }
                break;
            default:
            {
                if (m_newText.getSize() > MAXS_LEN)
                    return;
                m_newText += static_cast<wchar_t>(event.text.unicode);
            }
            }
        }
    }
    void setText(const sf::String& str)
    {
        m_text.setFillColor(sf::Color::Black);
        m_text.setString(str);
        m_str_text = str.toAnsiString();
        updateRect();
    }

};


class GraphicsTypes
{
private:

    sf::RectangleShape m_ok_btn;
    sf::Font m_font;
    std::string m_function;
    int m_koef[2] = { 1, 0 };
    sf::RenderWindow& m_win;

    // arr[0] - координаты, arr[1] - размеры
    int m_btn1_opt[2][2] = { {5, 100}, {60, 50} };

public:
    
    GraphicsTypes(sf::RenderWindow& main_window, sf::Font font) : m_win{ main_window }, m_font{ font }
    {
        
    }

    void setFunction(std::string function)
    {
        m_function = function;
    }

    void btn_and_text_draw()
    {
        m_ok_btn.setPosition(m_btn1_opt[0][0], m_btn1_opt[0][1]);
        m_ok_btn.setSize(sf::Vector2f(m_btn1_opt[1][0], m_btn1_opt[1][1]));
        m_ok_btn.setOutlineThickness(1);
        m_ok_btn.setOutlineColor(sf::Color::Black);
        m_win.draw(m_ok_btn);

        sf::Text ok_text("Ok", m_font, 40);
        ok_text.setPosition(10, 98);
        ok_text.setFillColor(sf::Color::Black);
        m_win.draw(ok_text);

    }

    void btn_click_event()
    {
        sf::Vector2i pos = sf::Mouse::getPosition(m_win);
        int x = pos.x;
        int y = pos.y;
        // std::cout << x << ' ' << y << std::endl;
        if ((x >= 5 && x <= 65) && (y >= 100 && y <= 150))     ok_btn_clicked();
    }

    void ok_btn_clicked()
    {
        new_main(m_function);
    }
};


class FocusController
{
public:
    FocusController(FocusObject* obj = 0) : m_object(obj)
    {
        if (m_object != 0)
        {
            m_object->setFocus();
        }
    }
    void setFocusObject(FocusObject* new_obj)
    {
        if (m_object == new_obj)
            return;
        if (m_object != 0)
            m_object->deleteFocus();
        if (new_obj != 0)
            new_obj->setFocus();
        m_object = new_obj;
    }
    FocusObject* getFocusObject()
    {
        return m_object;
    }

private:
    FocusObject* m_object;
};

FocusController fc;



int main()
{
    sf::RenderWindow app(sf::VideoMode(800, 600), L"SFML window");
    sf::Font font;
    app.clear(sf::Color::White);

    if (!font.loadFromFile("sfns-display-thin.ttf"))
    {
        return EXIT_FAILURE;
    }

    sf::Text txt;
    txt.setPosition(50, 50);
    txt.setFont(font);
    txt.setString(L"2x^2-2x+4");

    TextBox tb(txt, font);
    GraphicsTypes gt (app, font);

    fc.setFocusObject(&tb);
    while (app.isOpen())
    {
        sf::Event event;
        while (app.pollEvent(event))
        {
            // Close window : exit
            if (event.type == sf::Event::Closed)
                app.close();
            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.key.code == sf::Mouse::Left)
                {
                    gt.setFunction(tb.m_str_text);
                    gt.btn_click_event();
                }
            }
            FocusObject* fo = fc.getFocusObject();
            if (fo != 0)
            {
                fo->event(event);
            }
        }
        app.draw(tb);
        gt.btn_and_text_draw();
        app.display();
    }

}