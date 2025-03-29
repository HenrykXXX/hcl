[BaseContainerProps(configRoot: true)]
class HCL_FinancialInfoComponentClass : ScriptComponentClass
{
    [Attribute(defvalue: "1000", desc: "Starting account balance")]
    int m_iStartingAccountBalance;
    
    [Attribute(defvalue: "100", desc: "Starting personal money")]
    int m_iStartingPersonalMoney;
}

//------------------------------------------------------------------------------------------------
class HCL_FinancialInfoComponent : ScriptComponent
{
    protected HCL_FinancialInfoComponentClass m_pComponentClass;
    
    [RplProp(onRplName: "OnMoneyChanged")]
    protected int m_iAccountBalance;
    
    [RplProp(onRplName: "OnMoneyChanged")]
    protected int m_iPersonalMoney;
    
    //------------------------------------------------------------------------------------------------
    void HCL_FinancialInfoComponent(IEntityComponentSource src, IEntity ent, IEntity parent)
    {
        m_pComponentClass = HCL_FinancialInfoComponentClass.Cast(GetComponentData(ent));
        
        // Initialize starting values
        if (m_pComponentClass)
        {
            m_iAccountBalance = m_pComponentClass.m_iStartingAccountBalance;
            m_iPersonalMoney = m_pComponentClass.m_iStartingPersonalMoney;
        }
    }
    
    //------------------------------------------------------------------------------------------------
    // Get current account balance
    int GetAccountBalance()
    {
        return m_iAccountBalance;
    }
    
    //------------------------------------------------------------------------------------------------
    // Get current personal money
    int GetPersonalMoney()
    {
        return m_iPersonalMoney;
    }
    
    //------------------------------------------------------------------------------------------------
    // Deposit money from personal to account
    bool DepositMoney(int amount)
    {
        if (amount <= 0 || amount > m_iPersonalMoney)
            return false;
            
        m_iPersonalMoney -= amount;
        m_iAccountBalance += amount;
        
        // Notify about money change
        OnMoneyChanged();
        return true;
    }
    
    //------------------------------------------------------------------------------------------------
    // Withdraw money from account to personal
    bool WithdrawMoney(int amount)
    {
        if (amount <= 0 || amount > m_iAccountBalance)
            return false;
            
        m_iAccountBalance -= amount;
        m_iPersonalMoney += amount;
        
        // Notify about money change
        OnMoneyChanged();
        return true;
    }
    
    //------------------------------------------------------------------------------------------------
    // Called when money values change
    protected void OnMoneyChanged()
    {
        // You can add additional logic here if needed
        // For example, updating UI, playing sounds, etc.
    }
} 