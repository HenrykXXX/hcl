[BaseContainerProps()]
class HCL_SellItemAction: ScriptedUserAction
{
    // Component that contains item information
    protected HCL_SellableItemComponent m_ItemComponent;
    
    //------------------------------------------------------------------------------------------------
    override void Init(IEntity pOwnerEntity, GenericComponent pManagerComponent)
    {
        super.Init(pOwnerEntity, pManagerComponent);
        
        // Find the sellable item component
        m_ItemComponent = HCL_SellableItemComponent.Cast(pOwnerEntity.FindComponent(HCL_SellableItemComponent));
        if (!m_ItemComponent) 
        {
            Print("Error! Could not find Sellable Item Component for this action.", LogLevel.ERROR);
        }
    }
    
    //------------------------------------------------------------------------------------------------
    override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity) 
    {
		Print("dupa");
        if (!GetGame().InPlayMode())
            return;
            
        // Check if we have the item component
        if (!m_ItemComponent)
            return;
            
        // Try to sell the item
        m_ItemComponent.SellItem(pUserEntity);
    }
    
    //------------------------------------------------------------------------------------------------
    override bool CanBePerformedScript(IEntity user)
    {
        if (!user || !m_ItemComponent)
            return false;
            
        // Check if user has inventory manager
        SCR_InventoryStorageManagerComponent inventoryManager = SCR_InventoryStorageManagerComponent.Cast(user.FindComponent(SCR_InventoryStorageManagerComponent));
        if (!inventoryManager)
            return false;
            
        // Check if user has financial component
        HCL_FinancialInfoComponent financialComponent = HCL_FinancialInfoComponent.Cast(user.FindComponent(HCL_FinancialInfoComponent));
        if (!financialComponent)
            return false;
            
        // Check if user has the item in their inventory
        array<IEntity> items = new array<IEntity>();
        inventoryManager.GetAllItems(items, inventoryManager.GetCharacterStorage());
        
        foreach (IEntity item : items)
        {
			string itemName = item.GetPrefabData().GetPrefabName();
            // Check if this is the item we're looking for
            if (itemName == m_ItemComponent.GetItemName())
            {
                return true;
            }
        }
        
        return false;
    }
    
    //------------------------------------------------------------------------------------------------
    override bool CanBeShownScript(IEntity user)
    {
        // Always show the action
        return true;
    }
	
    
    //------------------------------------------------------------------------------------------------
    override bool GetActionNameScript(out string outName)
    {
        if (!m_ItemComponent)
        {
            outName = "Sell Item";
            return true;
        }
        
        // Get item name and value from component
        string itemName = m_ItemComponent.GetItemName();
        int value = m_ItemComponent.GetValue();
        
        outName = string.Format("Sell %1 (%2 money)", itemName, value);
        return true;
    }
	

    //------------------------------------------------------------------------------------------------
    override bool HasLocalEffectOnlyScript()
    {
        return true;
    }
} 