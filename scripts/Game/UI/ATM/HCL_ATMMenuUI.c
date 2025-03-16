class HCL_ATMMenuUI : ChimeraMenuBase
{
    protected HCL_ATMComponent m_ATMComponent;
    protected Widget m_wRoot;
    
    // Text widgets
    protected TextWidget m_wTitleText;
    protected TextWidget m_wWelcomeText;
    protected TextWidget m_wUserNameText;
    protected TextWidget m_wBalanceLabel;
    protected TextWidget m_wBalanceText;
    
    // Buttons
    protected ButtonWidget m_wGetCashButton;
    protected ButtonWidget m_wPayButton;
    protected ButtonWidget m_wDepositButton;
    protected ButtonWidget m_wOtherButton;
    protected ButtonWidget m_wExitButton;
    protected ButtonWidget m_wAmountButton;
    
    // Input
    protected EditBoxWidget m_wAmountInput;
    
    void SetATMComponent(HCL_ATMComponent atmComponent)
    {
        m_ATMComponent = atmComponent;
        
        if (m_ATMComponent)
        {
            UpdateDisplay();
        }
    }
    
    protected void UpdateDisplay()
    {
        if (!m_ATMComponent)
            return;
            
        if (m_wUserNameText)
            m_wUserNameText.SetText(m_ATMComponent.GetUserName());
            
        if (m_wBalanceText)
            m_wBalanceText.SetText(m_ATMComponent.GetBalance().ToString() + "$");
    }
    
    //------------------------------------------------------------------------------------------------
    override void OnMenuInit()
    {
        super.OnMenuInit();
        
        m_wRoot = GetRootWidget();
        
        // Get text widgets
        m_wTitleText = TextWidget.Cast(m_wRoot.FindAnyWidget("TitleText"));
        m_wWelcomeText = TextWidget.Cast(m_wRoot.FindAnyWidget("WelcomeText"));
        m_wUserNameText = TextWidget.Cast(m_wRoot.FindAnyWidget("UserNameText"));
        m_wBalanceLabel = TextWidget.Cast(m_wRoot.FindAnyWidget("BalanceLabel"));
        m_wBalanceText = TextWidget.Cast(m_wRoot.FindAnyWidget("BalanceText"));
        
        // Get button references
        m_wGetCashButton = ButtonWidget.Cast(m_wRoot.FindAnyWidget("GetCashButton"));
        m_wPayButton = ButtonWidget.Cast(m_wRoot.FindAnyWidget("PayButton"));
        m_wDepositButton = ButtonWidget.Cast(m_wRoot.FindAnyWidget("DepositButton"));
        m_wOtherButton = ButtonWidget.Cast(m_wRoot.FindAnyWidget("OtherButton"));
        m_wExitButton = ButtonWidget.Cast(m_wRoot.FindAnyWidget("ExitButton"));
        m_wAmountButton = ButtonWidget.Cast(m_wRoot.FindAnyWidget("AmountButton"));
        
        // Get input widget
        m_wAmountInput = EditBoxWidget.Cast(m_wRoot.FindAnyWidget("AmountInput"));
    }
    
    //------------------------------------------------------------------------------------------------
    override bool OnClick(Widget w, int x, int y, int button)
    {
        if (!m_ATMComponent)
            return false;
            
        if (w == m_wGetCashButton)
        {
            Print("Get Cash clicked!");
            return true;
        }
        else if (w == m_wPayButton)
        {
            Print("Pay clicked!");
            return true;
        }
        else if (w == m_wDepositButton)
        {
            Print("Deposit clicked!");
            return true;
        }
        else if (w == m_wOtherButton)
        {
            Print("Other clicked!");
            return true;
        }
        else if (w == m_wExitButton)
        {
            Close();
            return true;
        }
        else if (w == m_wAmountButton)
        {
            string amount = m_wAmountInput.GetText();
            Print("Amount entered: " + amount);
            return true;
        }
        
        return false;
    }
} 