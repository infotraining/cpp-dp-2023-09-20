

// Mati

#include <algorithm>
#include <functional>
#include <iostream>
#include <memory>
#include <vector>
#include <string>

using namespace std;

#define MOTIF

enum class IconType
{
    none,
    ok,
    cancel,
    warning,
    error
};

class Widget
{
public:
    virtual void draw() = 0;
    virtual ~Widget() = default;
};

class Button : public Widget
{
    std::string caption_;
    IconType icon_type_;

public:
    Button(const std::string& caption, IconType icon_type)
        : caption_{ caption }
        , icon_type_{ icon_type }
    {
    }

    std::string caption() const
    {
        return caption_;
    }

    IconType icon() const
    {
        return icon_type_;
    }
};

class Menu : public Widget
{
    std::string text_;

public:
    Menu(const std::string& text)
        : text_{ text }
    {
    }

    std::string text() const
    {
        return text_;
    }
};

class WidgetsFactory
{
public:
    virtual ~WidgetsFactory() {}
    virtual std::unique_ptr<Button> create_button(const std::string& caption, IconType icon_type) = 0;
    virtual std::unique_ptr<Menu> create_menu(const std::string& text) = 0;
};

class MotifButton : public Button
{
public:
    using Button::Button;

    void draw() override
    {
        cout << "MotifButton [ " << caption() << " ]\n";
    }
};

class MotifMenu : public Menu
{
public:
    using Menu::Menu;

    void draw() override
    {
        cout << "MotifMenu { " << text() << " }\n";
    }
};

class WindowsButton : public Button
{
public:
    using Button::Button;

    void draw() override
    {
        cout << "WindowsButton [ " << caption() << " ]\n";
    }
};

class WindowsMenu : public Menu
{
public:
    using Menu::Menu;

    void draw() override
    {
        cout << "WindowsMenu { " << text() << " }\n";
    }
};

class MotifWidgetsFactory : public WidgetsFactory
{
    std::unique_ptr<Button> create_button(const std::string& caption, IconType icon_type) override
    {
        return std::make_unique<MotifButton>(caption, icon_type);
    }
    std::unique_ptr<Menu> create_menu(const std::string& text) override
    {
        return std::make_unique<MotifMenu>(text);
    }
};

class WindowsWidgetsFactory : public WidgetsFactory
{
    std::unique_ptr<Button> create_button(const std::string& caption, IconType icon_type) override
    {
        return std::make_unique<WindowsButton>(caption, icon_type);
    }
    std::unique_ptr<Menu> create_menu(const std::string& text) override
    {
        return std::make_unique<WindowsMenu>(text);
    }
};

namespace AlternativeTake
{
    std::shared_ptr<WidgetsFactory>  getButtonFactory()
    {
#ifdef MOTIF
        return std::make_shared<MotifWidgetsFactory>();
#else 
        return std::make_shared<WindowsWidgetsFactory>();
#endif
    }

    // class WindowOne : public Window
    // {

    // public:
    //     WindowOne()
    //     {

    //         std::shared_ptr<WidgetsFactory> window = getButtonFactory();
    //         window->create_button("OK", IconType::ok);
    //         window->create_menu("File");
    //     }
    // };

}

class Window
{
    std::vector<std::unique_ptr<Widget>> widgets;

public:
    void display() const
    {
        std::cout << "######################\n";
        for (const auto& w : widgets)
            w->draw();
        std::cout << "######################\n\n";
    }

    void add_widget(std::unique_ptr<Widget> widget)
    {
        widgets.push_back(std::move(widget));
    }
};

class WindowOne : public Window
{
    std::shared_ptr<WidgetsFactory> widgets_factory_;
public:
    WindowOne(std::shared_ptr<WidgetsFactory> widgets_factory)
        : widgets_factory_(widgets_factory)
    {
        add_widget(widgets_factory->create_button("OK", IconType::ok));
        add_widget(widgets_factory->create_menu("File"));
    }
};

class WindowTwo : public Window
{
    std::shared_ptr<WidgetsFactory> widgets_factory_;
public:
    WindowTwo(std::shared_ptr<WidgetsFactory> widgets_factory)
        : widgets_factory_(widgets_factory)
    {
        add_widget(widgets_factory->create_menu("Edit"));
        add_widget(widgets_factory->create_button("OK", IconType::ok));
        add_widget(widgets_factory->create_button("Cancel", IconType::cancel));
    }
};

int main(void)
{
#ifdef MOTIF
    std::shared_ptr<WidgetsFactory> widgets_factory = std::make_shared<MotifWidgetsFactory>();
#else // WINDOWS
    std::shared_ptr<WidgetsFactory> widgets_factory = std::make_shared<WindowsWidgetsFactory>();
#endif

    WindowOne w1(widgets_factory);
    w1.display();

    WindowTwo w2(widgets_factory);
    w2.display();
}

