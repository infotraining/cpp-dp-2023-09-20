#include "fakeit.hpp"

#include <catch2/catch_test_macros.hpp>
#include <functional>
#include <algorithm>

using namespace fakeit;
using namespace std::literals;

using ListModel = std::vector<std::string>;

struct IListView
{
    virtual void ClearEditLine() = 0;
    virtual void AddItemToList(const std::string& item) = 0;
    virtual void RemoveItemFromList(int index) = 0;
    virtual void SetListItems(const std::vector<std::string>& items) = 0;
    virtual void EnableBtnAdd(bool enabled) = 0;
    virtual void EnableBtnRemove(bool enabled) = 0;
    virtual void ClearSelectionOnList() = 0;
    virtual bool SelectItemOnList(const std::string& item) = 0;
    virtual bool GetIndexOfSelectedItem() = 0;
    std::function<void(const std::string&)> ChangeTextEvent;
    std::function<void(const std::string&)> AddItemEvent;
    std::function<void()> RemoveItemEvent;
    virtual ~IListView() = default;
};

class ListPresenter
{
    ListModel& model_;
    IListView& view_;

public:
    ListPresenter(ListModel& model, IListView& view)
        : model_{model}
        , view_{view}
    {
        view_.ChangeTextEvent = [this](const std::string& txt) {
            OnTextChanged(txt);
        };

        view_.AddItemEvent = [this](const std::string& txt) {
            OnAddItem(txt);
        };

        view_.RemoveItemEvent = [this]() {
            OnRemoveItem();
        };
    }

    void Init()
    {
        view_.EnableBtnAdd(false);
        view_.EnableBtnRemove(false);
        view_.SetListItems(model_);
    }

    void OnTextChanged(const std::string& text)
    {        
        if (view_.SelectItemOnList(text))
        {
            view_.EnableBtnAdd(false);
            view_.EnableBtnRemove(true);
        }
        else
        {
            view_.ClearSelectionOnList();
            view_.EnableBtnAdd(true);
            view_.EnableBtnRemove(false);
        }
    }

    void OnAddItem(const std::string& item)
    {
        model_.push_back(item);
        view_.EnableBtnAdd(false);
        view_.ClearEditLine();
        view_.AddItemToList(item);
    }

    void OnRemoveItem()
    {
        int index = view_.GetIndexOfSelectedItem();
        assert(index >= 0);
        model_.erase(model_.begin() + index);
        view_.EnableBtnRemove(false);
        view_.ClearEditLine();
        view_.RemoveItemFromList(index);
    }
};

TEST_CASE("ListPresenter - init")
{
    ListModel model{"one", "two"};
    Mock<IListView> mock_view;
    When(Method(mock_view, EnableBtnAdd)).Return();
    When(Method(mock_view, EnableBtnRemove)).Return();
    When(Method(mock_view, SetListItems)).Return();
    When(Method(mock_view, ClearSelectionOnList)).Return();

    ListPresenter lst_presenter(model, mock_view.get());

    SECTION("initialization")
    {
        lst_presenter.Init();

        SECTION("button add is disabled")
        {
            Verify(Method(mock_view, EnableBtnAdd).Using(false));
        }

        SECTION("button remove is disabled")
        {
            Verify(Method(mock_view, EnableBtnRemove).Using(false));
        }

        SECTION("current list from model is displayed")
        {
            Verify(Method(mock_view, SetListItems).Using(std::vector{"one"s, "two"s}));
        }
    }
}

TEST_CASE("ListPresenter - change text event")
{
    ListModel model{"one", "two"};
    Mock<IListView> mock_view;
    IListView& view = mock_view.get();
    When(Method(mock_view, EnableBtnAdd)).Return();
    When(Method(mock_view, EnableBtnRemove)).Return();
    When(Method(mock_view, SetListItems)).Return();
    When(Method(mock_view, SelectItemOnList)).Return(true);
    When(Method(mock_view, ClearSelectionOnList)).Return();

    SECTION("select item is called")
    {
        When(Method(mock_view, SelectItemOnList)).Return(true);
        ListPresenter lst_presenter(model, view);
        std::string text = "a";
        view.ChangeTextEvent(text);
        Verify(Method(mock_view, SelectItemOnList).Using("a"));

        SECTION("add button is disabled")
        {
            Verify(Method(mock_view, EnableBtnAdd).Using(false));
        }

        SECTION("remove button is enabled")
        {
            Verify(Method(mock_view, EnableBtnRemove).Using(true));
        }
    }

    SECTION("when no matching item found")
    {
        When(Method(mock_view, SelectItemOnList).Using("a")).Return(false);
        ListPresenter lst_presenter(model, view);
        std::string text = "a";
        view.ChangeTextEvent(text);
        
        SECTION("then selection is cleared")
        {
            Verify(Method(mock_view, ClearSelectionOnList));
        }

        SECTION("add button is enabled")
        {
            Verify(Method(mock_view, EnableBtnAdd).Using(true));
        } 

        SECTION("remove button is disabled")
        {
            Verify(Method(mock_view, EnableBtnRemove).Using(false));
        }       
    }
}

TEST_CASE("ListPresenter - add item event")
{
    ListModel model{"one", "two"};
    Mock<IListView> mock_view;
    IListView& view = mock_view.get();
    When(Method(mock_view, EnableBtnAdd)).Return();
    When(Method(mock_view, EnableBtnRemove)).Return();
    When(Method(mock_view, SetListItems)).Return();
    When(Method(mock_view, SelectItemOnList)).Return(true);
    When(Method(mock_view, ClearSelectionOnList)).Return();
    When(Method(mock_view, ClearEditLine)).Return();
    When(Method(mock_view, AddItemToList)).Return();

    SECTION("when view raises AddItemEvent")
    {   
        ListPresenter lst_presenter(model, view);
        std::string text = "three";
        view.AddItemEvent(text);

        SECTION("item is added to model")
        {
            REQUIRE(model == std::vector{"one"s, "two"s, "three"s });
        }   

        SECTION("edit line is cleared")
        {
            Verify(Method(mock_view, ClearEditLine));
        }

        SECTION("add button is disabled")
        {
            Verify(Method(mock_view, EnableBtnAdd).Using(false));
        }

        SECTION("item is added to list on view")
        {
            Verify(Method(mock_view, AddItemToList).Using("three"));
        }
    }
}

TEST_CASE("ListPresenter - remove item event")
{
    ListModel model{"one"s, "two"s, "three"s};
    Mock<IListView> mock_view;
    IListView& view = mock_view.get();
    When(Method(mock_view, EnableBtnAdd)).Return();
    When(Method(mock_view, EnableBtnRemove)).Return();
    When(Method(mock_view, SetListItems)).Return();
    When(Method(mock_view, SelectItemOnList)).Return(true);
    When(Method(mock_view, ClearSelectionOnList)).Return();
    When(Method(mock_view, ClearEditLine)).Return();
    When(Method(mock_view, RemoveItemFromList)).Return();
    When(Method(mock_view, GetIndexOfSelectedItem)).Return(1);

    SECTION("when view raises RemoveItemEvent")
    {   
        ListPresenter lst_presenter(model, view);
        view.RemoveItemEvent();

        SECTION("item is removed from a model")
        {
            REQUIRE(model == std::vector{"one"s, "three"s });
        }   

        SECTION("edit line is cleared")
        {
            Verify(Method(mock_view, ClearEditLine));
        }

        SECTION("remove button is disabled")
        {
            Verify(Method(mock_view, EnableBtnRemove).Using(false));
        }

        SECTION("item is removed to list on view")
        {
            Verify(Method(mock_view, RemoveItemFromList).Using(1));
        }
    }
}