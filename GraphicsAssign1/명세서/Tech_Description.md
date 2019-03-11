# GameManager

게임 전반을 관리하는 매니저 클래스

Type: Class

Derived from: Monobehavior

## Constants 



## Fields

private Parser parser

private BoardManager boardManager

private DialogWindow DialogWindow

private UiManager uiManager

private BugManager bugManager

private SceneManager sceneManager

private ItemManager itemManager

private GameObject taskList

각 Manager class에 접근하기 위한 Fields

--------



## Properties



## Methods

- void Start()

  Field 초기화

- void Update()

  Tab키를 눌렀을 때 taskList가 보이게 함.

## Events



## Operators



# BoardManager

게임 전반의 맵과 플레이어/카메라의 위치를 관리하는 클래스

Type: Class

Derived from: Monobehavior

## Constants 



## Fields

private GameObject mainCamera

------



## Properties



## Methods

- void Start()

  Field 초기화

- void Update()

## Events



## Operators



# ScriptWindow

ScriptWindow를 관리하는 클래스

Type: Class

Derived from: Monobehavior

## Constructor



## Constants 



## Fields

private GameObject scriptWindow

private const float DELAYTIME=5.0f

ScriptWindow가 서서히 사라지게 하는 DelayTime.

private float passedTime=0

private bool IsWriteEventTriggered=true

private bool IsMouseOnScriptWindow=false

## Properties



## Methods

- private void Start()

  Field 초기화

- private void Update()

  ScriptWindow 위에 마우스가 올라가면 Script를 불투명화시켜준다.

- public void WriteALine( string str )

  ScriptWindow 에 한 줄을 적는다.

- public void ScriptWindowOn()

  ScriptWindow를 불투명화해주는 코드

- 

## Events



## Operators



# UiManager

UI를 관리하는 클래스

Type: Class

Derived from: Monobehavior

## Constructor



## Constants 



## Fields

private DialogWindow dialogWindow

private HorizontalLayoutGroup itemPanel

private Inventory inventory

private GameObject select

private bool isSelectBoxOn

## Properties



## Methods

public void OpenMessageBox( ItemManager.ItemList item, ItemManager.PresentState presentState )

public void MoveLeft()

현재 Scene에 따라 왼쪽 Scene으로 이동

public void MoveRight()

현재 Scene에 따라 오른쪽 Scene으로 이동

## Events



## Operators



# BugManager

버그를 관리하는 클래스

Type: Class

Derived from: Monobehavior

## Constructor



## Constants 



## Fields

[System.Flags]
public enum BugList 

Dictionary<BugList, bool> bugDic

버그를 통과할 때 마다 true를 대입한다.

## Properties



## Methods

public bool IsBugOvercomed(BugList bug)

public void BugOvercomed(BugList bug)

## Events



## Operators



# SceneManager

Scene을 관리하는 클래스

Type: Class

Derived from: Monobehavior

## Constructor



## Constants 



## Fields



## Properties



## Methods



## Events



## Operators



# TaskList

Tab을 눌렀을 때 나오는 작업 목록에 들어가는 Script

Type: Class

Derived from: Monobehavior

## Constructor



## Constants 



## Fields

private StringBuilder strBuilder

private TextMeshProUGUI taskListText

## Properties



## Methods

private void Start()

field를 초기화한다.

private void EditTaskList

TaskList의 내용을 바꿈.

private void AddStrikethrough

TaskList의 글에 취소선을 긋는다.

## Events



## Operators





# ItemManager



## Constructor



## Constants 



## Fields

[System.Flags]
    public enum ItemList { Scissors };

[System.Flags]

public enum PresentState {Default, Dropped, Gotten, Discarded};

Default는 플레이어의 인벤토리에 들어가지 않는 아이템의 상태이다.

## Properties



## Methods



## Events



## Operators



# Inventory



## Constructor



## Constants 

const int MAXITEMNUM = 20;

## Fields

public struct ItemElement { public ItemManager.ItemList item; public int num;}



private ItemElement[]  itemsInInventory

​	인벤토리 안의 아이템들을 가지고 있는 List



## Properties

public ItemElement[] ItemsInInventory { get => itemsInInventory; }

## Methods

public bool AddItem(ItemManager.ItemList itemList )

public bool RemoveItem( ItemManager.ItemList itemList )

public ItemElement CheckItemElement( ItemManager.ItemList itemList )

public bool CheckItem( ItemManager.ItemList itemList )

## Events



## Operators



# Item

게임에 사용되는 아이템들의 Base class. 게임 상의 Item GameObject의 Component로 붙는다.

플레이어의 인벤토리에 들어가지 않는 아이템들이 상속받고, 플레이어의 인벤토리에 들어가는 아이템들은 CollectableItem을 상속받는다. 

Type: Class

Derived from: Monobehavior

## Constructor



## Constants 



## Fields

public PresentState presentState;

​public ItemManager.ItemList item;

public delegate void ClickEventHandler( ItemManager.ItemList item, ItemManager.PresentState presentState, GameObject gObject );

item은 Clickevent가 발생한 아이템의 종류, presentState는 현재 클릭된 아이템의 상태(Dropped, Gotten 등), gObject는 클릭된 아이템의 gameObject를 인수로 넘긴다.

private UiManager uiManager;

## Properties



## Methods

protected virtual void start()

Field를 초기화한다.

protected virtual void update()

public virtual void Clicked()

Clickevent를 호출한다.

public virtual void ImageChange(ItemManager.ItemList item, ItemManager.PresentState presentState, GameObject gObject)

protected void ClickEventHandlerInvoker(ItemManager.ItemList item, ItemManager.PresentState presentState, GameObject gObject)

ClickEvent를 발동시키는 함수

protected void ClickEventHandlerReset()

ClickEvent를 리셋시키는 함수

## Events

public event ClickEventHandler ClickEvent;

아이템이 클릭되었을 때 해야할 반응을 Event로 처리한다.(현재 생각은 Inventory로 들어갔을 때 EventHandler를 바꿔주는 방식으로 할까 생각 중)

## Operators



# CollectableItem

플레이어의 Inventory에 들어가는 Item들이 상속받는 base Class이다.

Type: Class

Derived from: Item<-MonoBehavior

## Constructor



## Constants 



## Fields



## Properties



## Methods

protected override void start()

presentState를 Droped로 초기화하고, Inventory에 들어갔을 때 Item을 처리하기 위해(이미지 변화, Inventory에 아이템 추가) ClickEvent에 method를 추가한다.

protected override void update()

public override void Clicked()

아이템의 presentState에 따라 ClickEvent를 다르게 처리한다. Dropped일 경우 아이템을 줍는 Event가 발동되고, Gotten의 경우 아이템을 사용하는 이벤트를 발동한다.

public virtual void Discard()

public virtual void Mix()

public virtual void Use()

위의 세 method는 임시로 넣은 코드로, Dialogbox asset을 사면 거기에 맞추어 짤 예정.

## Events\



## Operators



# Parser

외부 텍스트 파일을 읽어들이는 클래스(현재 생각은 JSON을 써서 data 관리를 하는 것입니다. 안 되면 txt나 csv로...)

Type: Class

## Constructor

public Parser()

## Constants 



## Fields



## Properties



## Methods



## Events



## Operators

