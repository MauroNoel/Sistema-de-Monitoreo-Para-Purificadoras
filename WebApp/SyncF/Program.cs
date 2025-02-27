using DataLibrary;
using Microsoft.AspNetCore.Components;
using Microsoft.AspNetCore.Components.Web;
using SyncF.Data;
using Syncfusion.Blazor;

var builder = WebApplication.CreateBuilder(args);

// Add services to the container.
builder.Services.AddRazorPages();
builder.Services.AddServerSideBlazor();
builder.Services.AddSyncfusionBlazor();
builder.Services.AddSingleton<WeatherForecastService>();
builder.Services.AddSingleton<IDataAccess,DataAccess>();

var app = builder.Build();
Syncfusion.Licensing.SyncfusionLicenseProvider.RegisterLicense("MzU1MjQ2NkAzMjM3MmUzMDJlMzBDdS81RFVhcU9OZW5keW1KSzZIUTJZcmZIVEhSUGF2T0IyR29EcTdXYTlJPQ==");
// Configure the HTTP request pipeline.
if (!app.Environment.IsDevelopment())
{
    app.UseExceptionHandler("/Error");
    // The default HSTS value is 30 days. You may want to change this for production scenarios, see https://aka.ms/aspnetcore-hsts.
    app.UseHsts();
}

app.UseHttpsRedirection();

app.UseStaticFiles();

app.UseRouting();

app.MapBlazorHub();
app.MapFallbackToPage("/_Host");

app.Run();
