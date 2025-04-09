[BaseContainerProps()]
class HCL_SellableItemComponentClass: ScriptComponentClass
{
}

//------------------------------------------------------------------------------------------------
class HCL_SellableItemComponent: ScriptComponent
{
    // Value of the item when sold
    [Attribute("100", UIWidgets.EditBox, "Value of the item when sold")]
    protected int m_iValue;
    
    // Display name of the item
    [Attribute("Item", UIWidgets.EditBox, "Display name of the item")]
    protected string m_sItemName;
    
    // Category of the item (e.g., "Food", "Weapon", "Clothing")
    [Attribute("Misc", UIWidgets.EditBox, "Category of the item")]
    protected string m_sCategory;
    
    //------------------------------------------------------------------------------------------------
    int GetValue()
    {
        return m_iValue;
    }
    
    //------------------------------------------------------------------------------------------------
    string GetItemName()
    {
        return m_sItemName;
    }
    
    //------------------------------------------------------------------------------------------------
    string GetCategory()
    {
        return m_sCategory;
    }
    
    //------------------------------------------------------------------------------------------------
    bool SellItem(IEntity pPlayerEntity)
    {
        if (!pPlayerEntity)
            return false;
            
        // Get the player's inventory manager
        SCR_InventoryStorageManagerComponent inventoryManager = SCR_InventoryStorageManagerComponent.Cast(pPlayerEntity.FindComponent(SCR_InventoryStorageManagerComponent));
        if (!inventoryManager)
        {
            Print(string.Format("Error: You need an inventory to sell items."), LogLevel.ERROR);
            return false;
        }
        
        // Get the player's financial component
        HCL_FinancialInfoComponent financialComponent = HCL_FinancialInfoComponent.Cast(pPlayerEntity.FindComponent(HCL_FinancialInfoComponent));
        if (!financialComponent)
        {
            Print(string.Format("Error: You need a financial component to sell items."), LogLevel.ERROR);
            return false;
        }
        
        // Find the item in the player's inventory
        array<IEntity> items = new array<IEntity>();
        inventoryManager.GetAllItems(items, inventoryManager.GetCharacterStorage());
        
        bool itemFound = false;
        IEntity itemToRemove = null;
        
        foreach (IEntity item : items)
        {
			string itemName = item.GetPrefabData().GetPrefabName();
            // Check if this is the item we're looking for
            if (itemName == m_sItemName)
            {
                itemFound = true;
                itemToRemove = item;
                break;
            }
        }
        
        if (!itemFound)
        {
            Print(string.Format("Error: You don't have any %1 to sell.", m_sItemName), LogLevel.ERROR);
            return false;
        }
        
        // Remove the item from inventory
        if (!inventoryManager.TryRemoveItemFromInventory(itemToRemove))
        {
            Print(string.Format("Error: Failed to remove item from inventory."), LogLevel.ERROR);
            return false;
        }
        
        // Add money to the player's personal money
        financialComponent.AddMoney(m_iValue);
        
        // Show success message
        Print(string.Format("Success: Sold %1 for %2 money.", m_sItemName, m_iValue), LogLevel.NORMAL);
        
        // Show success hint
        SCR_HintManagerComponent.ShowCustomHint(string.Format("Sold %1 for %2 money", m_sItemName, m_iValue), "Item Sold", 3.0);
        
        return true;
    }
} 