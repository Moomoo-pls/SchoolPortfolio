public class Entity0 extends Entity
{    
	protected int[] directCost = new int[] {0, 1, 3, 7};
	protected int[] leastCost = new int[] {0, 1, 3, 7};
	protected int[] isDirect = new int[] {0, 1, 1, 1};
	
    // Perform any necessary initialization in the constructor
    public Entity0()
    {
		for(int i = 0; i < 4; i++)
		{
			for(int j = 0; j < 4; j++)
			{
				if(isDirect[i] == 1 && i == j)
					distanceTable[i][j] = directCost[i];
				else
					distanceTable[i][j] = 999;
			}
		}
		
		
		for(int i = 0; i < 4; i++)
		{
			if(isDirect[i] == 1)
			{
				Packet toSend = new Packet(0, i, leastCost);
				NetworkSimulator.toLayer2(toSend);
			}
		}
    }
    
    // Handle updates when a packet is received.  Students will need to call
    // NetworkSimulator.toLayer2() with new packets based upon what they
    // send to update.  Be careful to construct the source and destination of
    // the packet correctly.  Read the warning in NetworkSimulator.java for more
    // details.
    public void update(Packet p)
    {        
		boolean change = false;
		
		for(int i = 0; i < 4; i++)
		{
			if(i != 0)
			{
				int tempCost = directCost[p.getSource()] + p.getMincost(i);
				
				if(tempCost < distanceTable[i][p.getSource()])
				{
					distanceTable[i][p.getSource()] = tempCost;
					System.out.println("Updated Node 0's distance table.");
					printDT();
				}
				
				if(tempCost < leastCost[i])
				{
					change = true;
					leastCost[i] = tempCost;
				}
			}
		}
		
		if(change)
		{
			for(int i = 0; i < 4; i++)
			{
				if(isDirect[i] == 1)
				{
					Packet toSend = new Packet(0, i, leastCost);
					NetworkSimulator.toLayer2(toSend);
				}
			}
		}
    }
    
    public void linkCostChangeHandler(int whichLink, int newCost)
    {
    }
    
    public void printDT()
    {
        System.out.println();
        System.out.println("           via");
        System.out.println(" D0 |   1   2   3");
        System.out.println("----+------------");
        for (int i = 1; i < NetworkSimulator.NUMENTITIES; i++)
        {
            System.out.print("   " + i + "|");
            for (int j = 1; j < NetworkSimulator.NUMENTITIES; j++)
            {
                if (distanceTable[i][j] < 10)
                {    
                    System.out.print("   ");
                }
                else if (distanceTable[i][j] < 100)
                {
                    System.out.print("  ");
                }
                else 
                {
                    System.out.print(" ");
                }
                
                System.out.print(distanceTable[i][j]);
            }
            System.out.println();
        }
    }
}
