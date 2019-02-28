#pragma once

class TreeView : public winrt::Windows::UI::Xaml::Controls::TreeView
{
public:
	TreeView();

private:
	void FillRootElements();

	class TemplateSelector : public winrt::Windows::UI::Xaml::Controls::DataTemplateSelector
	{
	public:
		TemplateSelector();
		winrt::Windows::UI::Xaml::DataTemplate SelectTemplate(winrt::Windows::Foundation::IInspectable item);
	private:
		winrt::Windows::UI::Xaml::DataTemplate m_deviceTemplate = nullptr;
	};

	TemplateSelector m_templateSelector;
};