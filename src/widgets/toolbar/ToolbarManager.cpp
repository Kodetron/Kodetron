#include "ToolbarManager.h"

const QString ToolbarManager::TOGGLE_ICON_PATH = "icons/comparison.png";

ToolbarManager::ToolbarManager(QMainWindow *parent,
                               QWidget *outputContainer,
                               QWidget *examplesContainer,
                               QSplitter *inputOutputSplitter,
                               QSplitter *outputHorizontalSplitter,
                               QSplitter *editorIoSplitter)
    : QObject(parent)
    , m_mainWindow(parent)
    , m_outputContainer(outputContainer)
    , m_examplesContainer(examplesContainer)
    , m_inputOutputSplitter(inputOutputSplitter)
    , m_outputHorizontalSplitter(outputHorizontalSplitter)
    , m_editorIoSplitter(editorIoSplitter)
    , m_examplesVisible(false)
{
    setupToolbar();
}

ToolbarManager::~ToolbarManager(){}

void ToolbarManager::setupToolbar()
{
    // Creates the QDockWidget for the sidebar
    m_sidebarDockWidget = new QDockWidget(m_mainWindow);
    m_sidebarDockWidget->setAllowedAreas(Qt::LeftDockWidgetArea);
    m_sidebarDockWidget->setFeatures(QDockWidget::NoDockWidgetFeatures);

    // Creates a widget to hold content for the dock widget
    m_sidebarContentWidget = new QWidget(m_sidebarDockWidget);
    m_sidebarLayout = new QVBoxLayout(m_sidebarContentWidget);
    m_sidebarLayout->setContentsMargins(0, 0, 0, 0);
    m_sidebarLayout->setSpacing(0);

    // Setup the toggle button
    setupToggleButton();

    // Add stretch to push buttons to the top
    m_sidebarLayout->addStretch(1);

    // Set the content widget
    m_sidebarDockWidget->setWidget(m_sidebarContentWidget);

    // Add the dock widget to the main window's left dock area
    m_mainWindow->addDockWidget(Qt::LeftDockWidgetArea, m_sidebarDockWidget);

    // Set fixed width for the toolbar
    m_sidebarDockWidget->setFixedWidth(50);
}

void ToolbarManager::setupToggleButton()
{
    m_toggleOutputExamplesButton = new QToolButton(m_sidebarContentWidget);
    
    // Load custom icon
    QIcon customIcon(TOGGLE_ICON_PATH);
    
    // Fallback to standard icon if custom icon fails to load
    if (customIcon.isNull()) {
        customIcon = m_mainWindow->style()->standardIcon(QStyle::SP_FileDialogDetailedView);
    }
    
    m_toggleOutputExamplesButton->setIcon(customIcon);
    m_toggleOutputExamplesButton->setIconSize(QSize(32, 32));
    m_toggleOutputExamplesButton->setToolTip("Toggle Output Examples Window");
    m_toggleOutputExamplesButton->setToolButtonStyle(Qt::ToolButtonIconOnly);

    // Connect the button signal to the slot
    connect(m_toggleOutputExamplesButton, &QToolButton::clicked, 
            this, &ToolbarManager::toggleOutputExamples);

    // Add button to layout
    m_sidebarLayout->addWidget(m_toggleOutputExamplesButton);
}

void ToolbarManager::toggleOutputExamples()
{
    // Save current sizes to restore later
    QList<int> editorIoSizes = m_editorIoSplitter->sizes();
    QList<int> inputOutputSizes = m_inputOutputSplitter->sizes();
    
    if (!m_examplesVisible) {
        // Show examples: Replace single output with horizontal splitter
        
        // Remove output container from input_output_splitter
        m_outputContainer->setParent(nullptr);
        
        // Clear the horizontal splitter and add both containers
        while (m_outputHorizontalSplitter->count() > 0) {
            QWidget *w = m_outputHorizontalSplitter->widget(0);
            w->setParent(nullptr);
        }
        m_outputHorizontalSplitter->addWidget(m_outputContainer);
        m_outputHorizontalSplitter->addWidget(m_examplesContainer);
        
        // Add horizontal splitter to input_output_splitter
        m_inputOutputSplitter->addWidget(m_outputHorizontalSplitter);
        m_outputHorizontalSplitter->show();
        
        // Set equal sizes for the horizontal splitter
        m_outputHorizontalSplitter->setSizes(QList<int>({1, 1}));
        
        m_examplesVisible = true;
    } else {
        // Hide examples: Replace horizontal splitter with single output
        
        // Remove horizontal splitter from input_output_splitter
        m_outputHorizontalSplitter->setParent(nullptr);
        m_outputHorizontalSplitter->hide();
        
        // Remove output container from horizontal splitter
        m_outputContainer->setParent(nullptr);
        
        // Add output container back to input_output_splitter
        m_inputOutputSplitter->addWidget(m_outputContainer);
        
        m_examplesVisible = false;
    }
    
    // Restore the sizes to maintain code editor and input section sizes
    m_inputOutputSplitter->setSizes(inputOutputSizes);
    m_editorIoSplitter->setSizes(editorIoSizes);
}

#include "ToolbarManager.moc"