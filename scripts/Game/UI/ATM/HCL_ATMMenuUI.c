class HCL_ATMMenuUI : ChimeraMenuBase
{
    protected HCL_ATMComponent m_ATMComponent;
    protected HCL_FinancialInfoComponent m_FinancialComponent;
    protected Widget m_wRoot;
    
    // Text widgets
    protected TextWidget m_wTitleText;
    protected TextWidget m_wWelcomeText;
    protected TextWidget m_wUserNameText;
    protected TextWidget m_wAccountBalanceLabel;
    protected TextWidget m_wAccountBalanceText;
    protected TextWidget m_wPlayerBalanceLabel;
    protected TextWidget m_wPlayerBalanceText;
    protected TextWidget m_wStatusText;
    
    // Buttons
    protected ButtonWidget m_wWithdrawButton;
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
            // Get the player's financial component
            SCR_PlayerController playerController = SCR_PlayerController.Cast(GetGame().GetPlayerController());
            if (playerController)
            {
                IEntity playerEntity = playerController.GetMainEntity();
                if (playerEntity)
                {
                    m_FinancialComponent = HCL_FinancialInfoComponent.Cast(playerEntity.FindComponent(HCL_FinancialInfoComponent));
                }
            }
            
            UpdateDisplay();
        }
    }
    
    protected void UpdateDisplay()
    {
        if (!m_FinancialComponent)
            return;
            
        if (m_wUserNameText)
            m_wUserNameText.SetText(m_ATMComponent.GetUserName());
            
        if (m_wAccountBalanceText)
            m_wAccountBalanceText.SetText(m_FinancialComponent.GetAccountBalance().ToString() + "$");
            
        if (m_wPlayerBalanceText)
            m_wPlayerBalanceText.SetText(m_FinancialComponent.GetPersonalMoney().ToString() + "$");
    }
    
    protected void ShowStatus(string message, bool isError = false)
    {
        if (m_wStatusText)
        {
            m_wStatusText.SetText(message);
            //m_wStatusText.SetColor(isError ? ARGB(255, 255, 0, 0) : ARGB(255, 0, 255, 0));
        }
    }
    
    protected bool TryParseAmount(out int amount)
    {
        amount = 0;
        if (!m_wAmountInput)
            return false;
            
        string amountText = m_wAmountInput.GetText();
        if (amountText.IsEmpty())
            return false;
            
        amount = amountText.ToInt();
        return amount > 0;
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
        m_wAccountBalanceLabel = TextWidget.Cast(m_wRoot.FindAnyWidget("AccountBalanceLabel"));
        m_wAccountBalanceText = TextWidget.Cast(m_wRoot.FindAnyWidget("AccountBalanceText"));
        m_wPlayerBalanceLabel = TextWidget.Cast(m_wRoot.FindAnyWidget("PlayerBalanceLabel"));
        m_wPlayerBalanceText = TextWidget.Cast(m_wRoot.FindAnyWidget("PlayerBalanceText"));
        m_wStatusText = TextWidget.Cast(m_wRoot.FindAnyWidget("StatusText"));
        
        // Get button references
        m_wWithdrawButton = ButtonWidget.Cast(m_wRoot.FindAnyWidget("WithdrawButton"));
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
        if (!m_FinancialComponent)
        {
            ShowStatus("Error: Financial system not available", true);
            return false;
        }
            
        if (w == m_wWithdrawButton)
        {
            int amount;
            if (!TryParseAmount(amount))
            {
                ShowStatus("Please enter a valid amount", true);
                return true;
            }
            
            if (m_FinancialComponent.WithdrawMoney(amount))
            {
                ShowStatus("Successfully withdrew $" + amount);
                UpdateDisplay();
            }
            else
            {
                ShowStatus("Insufficient funds in account", true);
            }
            return true;
        }
        else if (w == m_wPayButton)
        {
            ShowStatus("Pay feature coming soon!");
            return true;
        }
        else if (w == m_wDepositButton)
        {
            int amount;
            if (!TryParseAmount(amount))
            {
                ShowStatus("Please enter a valid amount", true);
                return true;
            }
            
            if (m_FinancialComponent.DepositMoney(amount))
            {
                ShowStatus("Successfully deposited $" + amount);
                UpdateDisplay();
            }
            else
            {
                ShowStatus("Insufficient funds on hand", true);
            }
            return true;
        }
        else if (w == m_wOtherButton)
        {
            ShowStatus("Other features coming soon!");
            return true;
        }
        else if (w == m_wExitButton)
        {
            Close();
            return true;
        }
        else if (w == m_wAmountButton)
        {
            // Clear amount input
            if (m_wAmountInput)
                m_wAmountInput.SetText("");
            return true;
        }
        
        return false;
    }
} 