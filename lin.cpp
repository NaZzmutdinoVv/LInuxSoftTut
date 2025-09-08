#include <gtkmm.h>
#include <gtkmm/button.h>
#include <gtkmm/window.h>
#include <gtkmm/stylecontext.h>
#include <gtkmm/cssprovider.h>
#include <gtkmm/label.h>
#include <gtkmm/textview.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/entry.h>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <ctime>

class LinuxSoftTutWindow : public Gtk::Window {
public:
    LinuxSoftTutWindow();
    virtual ~LinuxSoftTutWindow();

protected:
    // Signal handlers:
    void on_button_terminal_clicked();
    void on_button_report_clicked();
    void load_css();
    std::string get_current_time();

    // Child widgets:
    Gtk::Box m_box;
    Gtk::Button m_button_terminal;
    Gtk::Button m_button_report;
    Gtk::Label m_label;
    Gtk::ScrolledWindow m_scrolled_window;
    Gtk::TextView m_textview;
    Gtk::Entry m_entry;
    Gtk::Label m_status_label;
};

LinuxSoftTutWindow::LinuxSoftTutWindow() 
: m_box(Gtk::ORIENTATION_VERTICAL, 10),
  m_button_terminal("Открыть терминал"),
  m_button_report("Отправить отчет об ошибке"),
  m_label("LinuxSoftTut - Изучение Linux"),
  m_status_label("Готово")
{
    // Window settings
    set_title("LinuxSoftTut");
    set_default_size(600, 500);
    set_border_width(10);

    // Load CSS styling
    load_css();

    // Add widgets to the box
    m_label.set_margin_top(20);
    m_label.set_margin_bottom(20);
    m_box.pack_start(m_label, Gtk::PACK_SHRINK);
    
    m_button_terminal.set_margin_bottom(10);
    m_box.pack_start(m_button_terminal, Gtk::PACK_SHRINK);
    
    m_entry.set_placeholder_text("Опишите ошибку или проблему...");
    m_entry.set_margin_bottom(10);
    m_box.pack_start(m_entry, Gtk::PACK_SHRINK);
    
    m_box.pack_start(m_button_report, Gtk::PACK_SHRINK);
    
    m_textview.set_editable(false);
    m_scrolled_window.add(m_textview);
    m_scrolled_window.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
    m_scrolled_window.set_margin_top(20);
    m_scrolled_window.set_margin_bottom(10);
    m_box.pack_start(m_scrolled_window);
    
    m_status_label.set_margin_bottom(10);
    m_box.pack_start(m_status_label, Gtk::PACK_SHRINK);

    // Connect signals
    m_button_terminal.signal_clicked().connect(sigc::mem_fun(*this, &LinuxSoftTutWindow::on_button_terminal_clicked));
    m_button_report.signal_clicked().connect(sigc::mem_fun(*this, &LinuxSoftTutWindow::on_button_report_clicked));

    // Add the box to the window
    add(m_box);

    // Show all children
    show_all_children();
    
    // Add welcome message
    Glib::RefPtr<Gtk::TextBuffer> buffer = m_textview.get_buffer();
    buffer->set_text("Добро пожаловать в LinuxSoftTut!\n\n"
                     "Это приложение поможет вам в изучении Linux.\n"
                     " - Нажмите 'Открыть терминал' для запуска терминала\n"
                     " - Опишите проблему и нажмите 'Отправить отчет об ошибке'\n\n"
                     "Все отчеты сохраняются в файле: /tmp/linuxsofttut_errors.log");
}

LinuxSoftTutWindow::~LinuxSoftTutWindow() {
}

void LinuxSoftTutWindow::load_css() {
    auto css = Gtk::CssProvider::create();
    auto style_context = get_style_context();
    
    // CSS styling for the application
    const char* css_data = R"(
        window {
            background-image: linear-gradient(to bottom, #0d1117, #161b22);
            color: #c9d1d9;
        }
        
        label {
            color: #58a6ff;
            font-size: 18px;
            font-weight: bold;
        }
        
        button {
            background-image: linear-gradient(to bottom, #238636, #2ea043);
            color: white;
            border: none;
            border-radius: 6px;
            padding: 10px 20px;
            font-size: 16px;
            transition: all 0.3s;
        }
        
        button:hover {
            background-image: linear-gradient(to bottom, #2ea043, #3fb950);
            box-shadow: 0 0 10px rgba(46, 160, 67, 0.7);
        }
        
        entry {
            background-color: #161b22;
            color: #c9d1d9;
            border: 1px solid #30363d;
            border-radius: 4px;
            padding: 8px;
            caret-color: #58a6ff;
        }
        
        textview {
            background-color: #0d1117;
            color: #8b949e;
            border: 1px solid #30363d;
            border-radius: 6px;
            padding: 10px;
            font-family: 'Fira Code', monospace;
        }
        
        .status-label {
            color: #8b949e;
            font-size: 12px;
        }
    )";
    
    try {
        css->load_from_data(css_data);
        style_context->add_provider_for_screen(Gdk::Screen::get_default(), css, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    } 
    catch (const Gtk::CssProviderError& error) {
        std::cerr << "CSS error: " << error.what() << std::endl;
    }
    
    // Add CSS class to status label
    auto status_context = m_status_label.get_style_context();
    status_context->add_class("status-label");
}

void LinuxSoftTutWindow::on_button_terminal_clicked() {
    m_status_label.set_text("Запуск терминала...");
    
    // Open terminal using xterm (common in Debian)
    int result = std::system("xterm &");
    
    if (result == 0) {
        // Add message to textview
        Glib::RefPtr<Gtk::TextBuffer> buffer = m_textview.get_buffer();
        Gtk::TextIter iter = buffer->end();
        buffer->insert(iter, "\n[" + get_current_time() + "] Терминал запущен\n");
        
        // Scroll to the bottom
        auto adj = m_scrolled_window.get_vadjustment();
        adj->set_value(adj->get_upper());
        
        m_status_label.set_text("Терминал запущен");
    } else {
        m_status_label.set_text("Ошибка запуска терминала. Установите xterm: sudo apt install xterm");
    }
}

void LinuxSoftTutWindow::on_button_report_clicked() {
    std::string report_text = m_entry.get_text();
    
    if (report_text.empty()) {
        m_status_label.set_text("Введите описание ошибки");
        return;
    }
    
    // Save report to file
    std::ofstream report_file("/tmp/linuxsofttut_errors.log", std::ios_base::app);
    if (report_file.is_open()) {
        report_file << "[" << get_current_time() << "] " << report_text << std::endl;
        report_file.close();
        
        // Add message to textview
        Glib::RefPtr<Gtk::TextBuffer> buffer = m_textview.get_buffer();
        Gtk::TextIter iter = buffer->end();
        buffer->insert(iter, "[" + get_current_time() + "] Отправлен отчет: " + report_text + "\n");
        
        // Scroll to the bottom
        auto adj = m_scrolled_window.get_vadjustment();
        adj->set_value(adj->get_upper());
        
        m_entry.set_text("");
        m_status_label.set_text("Отчет сохранен в /tmp/linuxsofttut_errors.log");
    } else {
        m_status_label.set_text("Ошибка открытия файла для записи");
    }
}

std::string LinuxSoftTutWindow::get_current_time() {
    std::time_t now = std::time(nullptr);
    std::tm* ptm = std::localtime(&now);
    char buffer[32];
    std::strftime(buffer, 32, "%Y-%m-%d %H:%M:%S", ptm);
    return std::string(buffer);
}

int main(int argc, char* argv[]) {
    auto app = Gtk::Application::create(argc, argv, "org.linuxsofttut.app");
    
    LinuxSoftTutWindow window;
    
    // Set window icon if possible
    try {
        window.set_icon_from_file("/usr/share/icons/hicolor/48x48/apps/utilities-terminal.png");
    } 
    catch (const Gtk::IconThemeError& error) {
        // Use default icon if specific icon not found
        std::cerr << "Icon not found: " << error.what() << std::endl;
    }
    catch (const Glib::FileError& error) {
        std::cerr << "File error: " << error.what() << std::endl;
    }
    
    return app->run(window);
}